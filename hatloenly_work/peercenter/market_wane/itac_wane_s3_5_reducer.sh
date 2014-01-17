#!/bin/bash
# @file itac_wane_s3_5.sh
# @author hele01(hele01@baidu.com)
# @version 1.0
# @date 2014-01-17 10:42:47
# @brief 市场风向标log平台线上脚本itac_wane_s3_5，reducer升级
#   输入数据1：wane_s3_out_3(query | tradeid | time | region | pv)
#   输入数据2：wane_s3_out_4(query | tradeid | wordid | time | region | epv)
#   mapper：交换第一列和第二列
#
#   输出9个文件：
#     A(tradeid | epv)
#     B(tradeid | pv)
#     C(tradeid | epv * 10000 / pv)
#     D(tradeid | time | epv)
#     E(tradeid | time | pv)
#     F(tradeid | time | epv * 10000 / pv)
#     G(tradeid | region | epv)
#     H(tradeid | region | pv)
#     I(tradeid | region | epv * 10000 / pv)


awk '
BEGIN {
    FS = OF = "\t";
    tradeid = "";
    query = "";
    epv = 0;
    pv = 0;
}
{
    # 第一行不输出
    if (tradeid == "") {
        tradeid = $1;
        query = $2;
        epv = 0;
        pv = 0;
    # 如果新的tradeid，则输出结果
    } else if (tradeid != $1) {
        printf("%s\t%d#A\n", tradeid, epv);
        printf("%s\t%d#B\n", tradeid, pv);
        printf("%s\t%d#C\n", tradeid, epv * 10000 / pv);
        for (time in time_pv_dict) {
            time_epv = time_epv_dict[time];
            time_pv = time_pv_dict[time];
            time_sta = time_epv * 10000 / time_pv;
            printf("%s\t%d\t%d#D\n", tradeid, time + 1, time_epv);
            printf("%s\t%d\t%d#E\n", tradeid, time + 1, time_pv);
            printf("%s\t%d\t%d#F\n", tradeid, time + 1, time_sta);
        }
        for (region in region_pv_dict) {
            region_epv = region_epv_dict[region];
            region_pv = region_pv_dict[region];
            region_sta = region_epv * 10000 / region_pv
            printf("%s\t%d\t%d#G\n", tradeid, region, region_epv);
            printf("%s\t%d\t%d#H\n", tradeid, region, region_pv);
            printf("%s\t%d\t%d#I\n", tradeid, region, region_sta);
        }
        delete time_epv_dict;
        delete time_pv_dict;
        delete region_epv_dict;
        delete region_pv_dict;

        tradeid = $1;
        query = $2;
        epv = 0;
        pv = 0;
    }

    # 来自wane_s3_out_3的pv数据，每行最后一个字段pv的值总是1
    if (NF == 5) {
        pv += 1;
        time_pv_dict[$3] += 1;
        region_pv_dict[$4] += 1;
        next;
    }
    # 来自wane_s3_out_4的epv数据，每行最后一个字段epv的值总是1
    if (NF == 6) {
        epv += 1;
        time_epv_dict[$4] += 1;
        region_epv_dict[$5] += 1;
        next;
    }
}
END {
    # 打印最后一次结果
    printf("%s\t%d#A\n", tradeid, epv);
    printf("%s\t%d#B\n", tradeid, pv);
    printf("%s\t%d#C\n", tradeid, epv * 10000 / pv);
    for (time in time_pv_dict) {
        time_epv = time_epv_dict[time];
        time_pv = time_pv_dict[time];
        time_sta = time_epv * 10000 / time_pv;
        printf("%s\t%d\t%d#D\n", tradeid, time + 1, time_epv);
        printf("%s\t%d\t%d#E\n", tradeid, time + 1, time_pv);
        printf("%s\t%d\t%d#F\n", tradeid, time + 1, time_sta);
    }
    for (region in region_pv_dict) {
        region_epv = region_epv_dict[region];
        region_pv = region_pv_dict[region];
        region_sta = region_epv * 10000 / region_pv
        printf("%s\t%d\t%d#G\n", tradeid, region, region_epv);
        printf("%s\t%d\t%d#H\n", tradeid, region, region_pv);
        printf("%s\t%d\t%d#I\n", tradeid, region, region_sta);
    }
    delete time_epv_dict;
    delete time_pv_dict;
    delete region_epv_dict;
    delete region_pv_dict;
} '
