# coding: UTF-8
##
# @file backup_data_in_hadoop.py
# @author hele01(hele01@baidu.com)
# @version 1.0
# @date 2014-01-17 10:30:28
# @brief hadoop关键数据备份脚本，用distcp实现

import commands
import datetime
import sys

# 本地hadoop客户端路径
HADOOP_PATH = '/home/hele/hadoop/hadoop_nj/hadoop/bin/hadoop'
# distcp参数
DISTCP_FLAG = '-update'

# 源主机、端口和路径
SRC_HOST = 'hdfs://nmg01-khan-hdfs.dmop.baidu.com'
SRC_PORT = 54310
SRC_PATH = '/app/ns/lsp/output/ecom_darwin/pvstat_mainfunc_query_day'

# 目的主机、端口和路径
DST_HOST = 'hdfs://nmg01-khan-hdfs.dmop.baidu.com'
DST_PORT = 54310
DST_PATH = '/app/ecom/itac/hele01/test_dir4'

# 粒度，day表示天粒度，hour表示小时粒度
# 粒度和文件名有关，天粒度文件名yyyymmdd，小时粒度文件名yyyymmddHH
GRANULARITY = 'day'
# 从第几天（或者小时）前开始
TIME_AGO = 7
# 总共几天（或者小时）
NTIME = 5

# 错误码
SUCCESS = 0
ERROR_HADOOP_PATH = 1
ERROR_SRC_PATH_NOT_EXIST = 2
ERROR_DST_PATH_NOT_EXIST = 3
ERROR_GRANULARITY = 4


def backup_data_process():
    check_dst_path_exist_command = '{hadoop_path} fs -test -e {dst_host}:{dst_port}{dst_path}'.format(
            hadoop_path = HADOOP_PATH,
            dst_host = DST_HOST,
            dst_port = DST_PORT,
            dst_path = DST_PATH)
    check_src_path_exist_command = '{hadoop_path} fs -test -e {src_host}:{src_port}{src_path}'.format(
            hadoop_path = HADOOP_PATH,
            src_host = SRC_HOST,
            src_port = SRC_PORT,
            src_path = SRC_PATH)
    status, output = commands.getstatusoutput(check_src_path_exist_command)
    if status != 0 and (output.find('command not found') != -1 or output.find('No such file or directory') != -1):
        print 'FATIL: command not found, check HADOOP_PATH: {hadoop_path}'.format(hadoop_path = HADOOP_PATH)
        return ERROR_HADOOP_PATH
    if status != 0:
        print 'FATIL: src_path {src_host}:{src_port}{src_path} not exist'.format(src_host =SRC_HOST, src_port = SRC_PORT, src_path = SRC_PATH)
        return ERROR_SRC_PATH_NOT_EXIST
    status, output = commands.getstatusoutput(check_dst_path_exist_command)
    if status != 0:
        print 'FATIL: dst_path {dst_host}:{dst_port}{dst_path} not exist'.format(dst_host = DST_HOST, dst_port = DST_PORT, dst_path = DST_PATH)
        return ERROR_DST_PATH_NOT_EXIST

    now = datetime.datetime.now()
    for days_ago in range(TIME_AGO - NTIME + 1, TIME_AGO + 1):
        time = ''
        if GRANULARITY == 'day':
            delta = datetime.timedelta(days = days_ago)
            time = (now - delta).strftime('%Y%m%d')
        elif GRANULARITY == 'hour':
            delta = datetime.timedelta(hours = days_ago)
            time = (now - delta).strftime('%Y%m%d%H')
        else:
            print 'FATIL: unknow GRANULARITY use "day" or "hour"'
            return ERROR_GRANULARITY
        check_file_exist_command = '{hadoop_path} fs -test -e {src_host}:{src_port}{src_path}/{file_name}'.format(
                hadoop_path = HADOOP_PATH,
                src_host = SRC_HOST,
                src_port = SRC_PORT,
                src_path = SRC_PATH,
                file_name = time)
        status, output = commands.getstatusoutput(check_file_exist_command)
        if status != 0:
            print 'ERROR file {file_name} at {src_host}:{src_port}{src_path} not exit'.format(
                hadoop_path = HADOOP_PATH,
                src_host = SRC_HOST,
                src_port = SRC_PORT,
                src_path = SRC_PATH,
                file_name = time)
            continue
        distcp_command = '{hadoop_path} distcp {distcp_flag} {src_host}:{src_port}{src_path}/{file_name} {dst_host}:{dst_port}{dst_path}/{file_name}'.format(
                hadoop_path = HADOOP_PATH,
                distcp_flag = DISTCP_FLAG,
                src_host = SRC_HOST,
                src_port = SRC_PORT,
                src_path = SRC_PATH,
                file_name = time,
                dst_host = DST_HOST,
                dst_port = DST_PORT,
                dst_path = DST_PATH)
        status, output = commands.getstatusoutput(distcp_command)
        if status != 0:
            print 'ERROR: {command} fail'.format(command = distcp_command)
        else:
            print 'NOTICE: {command} success'.format(command = distcp_command)
    return SUCCESS

def main():
    status = backup_data_process()
    sys.exit(status)

if __name__ == '__main__':
    main()

