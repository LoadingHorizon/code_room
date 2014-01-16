// Copyright (c) LoadingHorizon
// @file geometry.cpp
// @brief 无聊写着玩的，主要是一些和立体几何有关的东西
// @author hatlonely@foxmail.com
// @version 1.0
// @date 2014-01-16 12:00:49

#include <stdio.h>

#include <iostream>
#include <cmath>
#include <string>

namespace lh {

const double kPrecision = 1e-10;
bool equal(double d1, double d2) {
    return (d1 > d2 - kPrecision && d1 < d2 + kPrecision);
}


// 点
class LHPoint {
public:
    LHPoint(double x = 0.0, double y = 0.0, double z = 0.0) {
        _x = x;
        _y = y;
        _z = z;
    }
    double distance_to(const LHPoint &p) {
        double dx = _x - p._x;
        double dy = _y - p._y;
        double dz = _z - p._z;
        return sqrt(dx * dx + dy * dy + dz * dz);
    }
    double get_x() const {
        return _x;
    }
    double get_y() const {
        return _y;
    }
    double get_z() const {
        return _z;
    }
    void set_x(double x) {
        _x = x;
    }
    void set_y(double y) {
        _y = y;
    }
    void set_z(double z) {
        _z = z;
    }
    std::string to_string() {
        char carray[kStrLen];
        snprintf(carray, kStrLen, "(%lg, %lg, %lg)", _x, _y, _z);
        return std::string(carray);
    }

    static const size_t kStrLen;
private:
    double _x;
    double _y;
    double _z;
};
const size_t LHPoint::kStrLen = 256;


// 向量
class LHVector {
public:
    LHVector(double x = 0.0, double y = 0.0, double z = 0.0) {
        _x = x;
        _y = y;
        _z = z;
    }
    double product(const LHVector &v) {
        return (_x * v._x + _y * v._y + _z * v._z);
    }
    LHVector operator +(const LHVector &v) {
        return LHVector(_x + v._x, _y + v._y, _z + v._z);
    }
    LHVector operator -(const LHVector &v) {
        return LHVector(_x - v._x, _y - v._y, _z - v._z);
    }
    LHVector operator *(double d) {
        return LHVector(_x * d, _y * d, _z * d);
    }
    LHVector operator *(const LHVector &v) {
        return LHVector(_y * v._z - v._y * _z, 
                _z * v._x - v._z * _x, _x * v._y - v._x * _y);
    }
    double module() const {
        return sqrt(_x * _x + _y * _y + _z * _z);
    }
    double get_x() const {
        return _x;
    }
    double get_y() const {
        return _y;
    }
    double get_z() const {
        return _z;
    }
    void set_x(double x) {
        _x = x;
    }
    void set_y(double y) {
        _y = y;
    }
    void set_z(double z) {
        _z = z;
    }
    std::string to_string() {
        char carray[kStrLen];
        snprintf(carray, kStrLen, "%lgi + %lgj + %lgk", _x, _y, _z);
        return std::string(carray);
    }

    static const size_t kStrLen;
private:
    double _x;
    double _y;
    double _z;
};
const size_t LHVector::kStrLen = 256;


// 用两点表示一条直线
// (x - x1) / (x2 - x1) = (y - y1) / (y2 - y1) = (z - z1) / (z2 -z1)
class LHLine {
public:
    LHLine(const LHPoint &pa, const LHPoint &pb): _pointa(pa), _pointb(pb) {}
    bool contains_point(const LHPoint &p) {
        double ratio_x = (p.get_x() - _pointa.get_x()) / (_pointb.get_x() - _pointa.get_x());
        double ratio_y = (p.get_y() - _pointa.get_y()) / (_pointb.get_y() - _pointa.get_y());
        double ratio_z = (p.get_z() - _pointa.get_z()) / (_pointb.get_z() - _pointa.get_z());
        return (equal(ratio_x, ratio_y) && equal(ratio_y, ratio_z));
    }
    // LHPoint intersection_point(const LHLine &l) {
        // TODO
        // 两直线的交点
    // }
    // double distance_to(const LHPoint &p) {
        // TODO
        // 点到直线的距离
    // }
    LHPoint get_pointa() const {
        return _pointa;
    }
    LHPoint get_pointb() const {
        return _pointb;
    }
    std::string to_string() {
        char carray[kStrLen];
        snprintf(carray, kStrLen, "%s, %s", 
                _pointa.to_string().c_str(), _pointb.to_string().c_str());
        return std::string(carray);
    }

    static const size_t kStrLen;
private:
    LHPoint _pointa;
    LHPoint _pointb;
};
const size_t LHLine::kStrLen = LHPoint::kStrLen * 2 + 2;


// 平面的一般表示
// ax + by + cz + d = 0
class LHPlane {
public:
    LHPlane(double a, double b, double c, double d) {
        double module = sqrt(a * a + b * b + c * c);
        if (equal(module, 0.0)) {
            _a = _b = _c = _d = 0.0;
        } else {
            _a = a / module;
            _b = b / module;
            _c = c / module;
            _d = d / module;
        }
    }
    LHPlane(const LHPoint &p, const LHVector &v) {
        double module = v.module();
        _a = v.get_x() / module;
        _b = v.get_y() / module;
        _c = v.get_z() / module;
        _d = 0 - p.get_x() * _a - p.get_y() * _b - p.get_z() * _c;
    }
    // LHPlane(const LHPoint &pa, const LHPoint &pb, const LHPoint &pc) {
        // TODO
        // 三点确定一个平面
    // }
    // 法向量
    LHVector nomal_vector() {
        return LHVector(_a, _b, _c);
    }
    // LHLine intersection_line(const LHPlane &pl) {
        // TODO
        // 两个平面的交线
    // }
    // LHPoint intersection_point(const LHLine &l) {
        // TODO
        // 线与面的交点
    // }
    // double distance_to(const LHPoint &p) {
        // TODO
        // 点到平面的距离
    // }
    bool contains_point(const LHPoint &p) {
        return equal(p.get_x() * _a + p.get_y() * _b + p.get_z() * _c + _d, 0.0);
    }
    bool contains_line(const LHLine &l) {
        return (contains_point(l.get_pointa()) && contains_point(l.get_pointb()));
    }
    std::string to_string() {
        char carray[kStrLen];
        snprintf(carray, kStrLen, "%lex + %ley + %lez + %le = 0", 
                _a, _b, _c, _d);
        return std::string(carray);
    }

    static const size_t kStrLen;
private:
    double _a;
    double _b;
    double _c;
    double _d;
};
const size_t LHPlane::kStrLen = 256;


class LHTriangle {
public:
    LHTriangle(const LHPoint &pa, const LHPoint &pb, const LHPoint &pc):
        _pointa(pa), _pointb(pb), _pointc(pc) {}
    double a() {
        return _pointb.distance_to(_pointc);
    }
    double b() {
        return _pointc.distance_to(_pointa);
    }
    double c() {
        return _pointa.distance_to(_pointb);
    }
    // double area() {
        // TODO
        // 三角形面积
    // }
    // LHCircle incircle() {
        // TODO
        // 内接圆
    // }
    // LHCircle circumcircle() {
        // TODO
        // 外接圆
    // }
    LHPoint gravity_center() {
        double x = (_pointa.get_x() + _pointb.get_x() + _pointc.get_x()) / 3;
        double y = (_pointa.get_y() + _pointb.get_y() + _pointc.get_y()) / 3;
        double z = (_pointa.get_z() + _pointb.get_z() + _pointc.get_z()) / 3;
        return LHPoint(x, y, z);
    }
    // LHPoint circum_center() {
        // TODO
        // 外心
    // }
    // LHPoint in_center() {
        // TODO
        // 内心
    // }
    // LHPoint ortho_center() {
        // TODO
        // 垂心
    // }
    // LHPoint es_center() {
        // TODO
        // 旁心
    // }
    std::string to_string() {
        char carray[kStrLen];
        snprintf(carray, kStrLen, "%s, %s, %s", 
                _pointa.to_string().c_str(), 
                _pointb.to_string().c_str(), 
                _pointc.to_string().c_str());
        return std::string(carray);
    }

    static const size_t kStrLen;
private:
    LHPoint _pointa;
    LHPoint _pointb;
    LHPoint _pointc;
};
const size_t LHTriangle::kStrLen = LHPoint::kStrLen * 3 + 4;


}  // namespace_lh

int main() {
    using namespace lh;
    LHPoint point[3];
    point[0] = LHPoint(1, 0, 0);
    point[1] = LHPoint(0, -2, 0);
    point[2] = LHPoint(0, 0, -3);
    LHLine line(point[0], point[1]);
    LHTriangle triangle(point[0], point[1], point[2]);
    std::cout << triangle.to_string() << std::endl;
    std::cout << triangle.a() << std::endl;
    std::cout << triangle.b() << std::endl;
    std::cout << triangle.c() << std::endl;
    std::cout << triangle.gravity_center().to_string() << std::endl;
    std::cout << line.get_pointa().to_string() << std::endl;

    return 0;
}
