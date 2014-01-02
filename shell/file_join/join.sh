#!/bin/bash

# @file join.sh
# @brief 文件join操作  
# @author hatlonely@foxmail.com
# @version 1.0
# @date 2014-01-02

# 背景：
#   数据库中join操作可以把两张表的信息合在一起
#   文件也有类似的操作，把两个文件按照某列合在一起
# 问题描述：
#   输入：
#       有多个文件，文件分为若干列，列与列之间以\t分割
#       其中某几列可以唯一标识一行（主键）
#       文件有相同的主键
#   输出：
#       将两个文件按某些列合并后的文件
# 思路：
#   1、给每个文件添加一列作为文件标识（合成一个文件后，用来标识是来自哪个文件的数据） <= awk
#   2、将所有文件合并并排序（排序后主键相同的行相邻了，并且其顺序和步骤1中的文件标识有关） <= sort
#   3、处理排序后的文件，合并主键相同的行 <= awk


# 例子：
#   将./data目录下book_author和book_price和并成book_info到result目录下
#   book_author的格式为book\tauthor，book_price的格式为book\tprice
#   合并后book_info格式为book\tauthor\tbook_price

DATADIR="./data"
INFILE1="book_author"
INFILE2="book_price"

RESULTDIR="./result"
OUTFILE="book_info"

# 1、添加文件标识
#   给book_author加一列0
#   给book_price加一列1
awk 'BEGIN{ FS = OFS = "\t" } { print $1, 0, $2 }' $DATADIR/$INFILE1 > $INFILE1.mark
awk 'BEGIN{ FS = OFS = "\t" } { print $1, 1, $2 }' $DATADIR/$INFILE2 > $INFILE2.mark

# 2、将所有文件合并排序
sort $INFILE1.mark $INFILE2.mark > $OUTFILE.tmp.sort

# 3、合并主键相同的行
awk 'BEGIN{ FS = OFS = "\t"; book = ""; author = "" } { 
    if ($2 == 0) {
        book = $1; 
        author = $3; 
        next;
    } else if ($2 == 1) { 
        if ($1 == book) { 
            print $1, author, $3 } 
        } 
    }' $OUTFILE.tmp.sort > $RESULTDIR/$OUTFILE

# 清理中间文件
rm *.mark
rm *.sort
