#include "Matrix.hpp"
#include <catch2/catch_test_macros.hpp>
#include <utility>

TEST_CASE("Constructing and inspecting a 4x4 matrix", "[Matrix]") {
  RT::Matrix<4> m = RT::Matrix<4>(
      {1, 2, 3, 4, 5.5, 6.5, 7.5, 8.5, 9, 10, 11, 12, 13.5, 14.5, 15.5, 16.5});
  REQUIRE(m(0, 0) == 1);
  REQUIRE(m(0, 3) == 4);
  REQUIRE(m(1, 0) == 5.5);
  REQUIRE(m(1, 2) == 7.5);
  REQUIRE(m(2, 2) == 11);
  REQUIRE(m(3, 0) == 13.5);
  REQUIRE(m(3, 2) == 15.5);
  m(3, 2) = 666;
  REQUIRE(m(3, 2) == 666);
}

TEST_CASE("A 2x2 matrix ought to be representable", "[Matrix]") {
  RT::Matrix<2> m = RT::Matrix<2>({-3, 5, 1, -2});
  REQUIRE(m(0, 0) == -3);
  REQUIRE(m(0, 1) == 5);
  REQUIRE(m(1, 0) == 1);
  REQUIRE(m(1, 1) == -2);
}

TEST_CASE("A 3x3 matrix ought to be representable", "[Matrix]") {
  RT::Matrix<3> m = RT::Matrix<3>({-3, 5, 0, 1, -2, -7, 0, 1, 1});
  REQUIRE(m(0, 0) == -3);
  REQUIRE(m(1, 1) == -2);
  REQUIRE(m(2, 2) == 1);
}

TEST_CASE("Matrix equality with identical matrices", "[Matrix]") {
  RT::Matrix<4> a =
      RT::Matrix<4>({1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2});
  RT::Matrix<4> b =
      RT::Matrix<4>({1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2});
  REQUIRE(a == b);
}

TEST_CASE("Matrix equality with different matrices", "[Matrix]") {
  RT::Matrix<4> a =
      RT::Matrix<4>({1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2});
  RT::Matrix<4> b =
      RT::Matrix<4>({2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1});
  REQUIRE(a != b);
}

TEST_CASE("Multiplying two matrices", "[Matrix]") {
  RT::Matrix<4> a =
      RT::Matrix<4>({1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2});
  RT::Matrix<4> b =
      RT::Matrix<4>({-2, 1, 2, 3, 3, 2, 1, -1, 4, 3, 6, 5, 1, 2, 7, 8});
  RT::Matrix<4> expected = RT::Matrix<4>(
      {20, 22, 50, 48, 44, 54, 114, 108, 40, 58, 110, 102, 16, 26, 46, 42});
  REQUIRE(a * b == expected);
}

TEST_CASE("A matrix multiplied by a tuple", "[Matrix]") {
  RT::Matrix<4> a =
      RT::Matrix<4>({1, 2, 3, 4, 2, 4, 4, 2, 8, 6, 4, 1, 0, 0, 0, 1});
  RT::Tuple b = RT::Tuple(1, 2, 3, 1);
  RT::Tuple expected = RT::Tuple(18, 24, 33, 1);
  REQUIRE(a * b == expected);
}

TEST_CASE("Multiplying a matrix by the identity matrix", "[Matrix]") {
  RT::Matrix<4> a =
      RT::Matrix<4>({0, 1, 2, 4, 1, 2, 4, 8, 2, 4, 8, 16, 4, 8, 16, 32});
  RT::Matrix<4> expected =
      RT::Matrix<4>({0, 1, 2, 4, 1, 2, 4, 8, 2, 4, 8, 16, 4, 8, 16, 32});
  REQUIRE(a * RT::identityMatrix<4>() == expected);
}

TEST_CASE("Multiplying the identity matrix by a tuple", "[Matrix]") {
  RT::Tuple a = RT::Tuple(1, 2, 3, 4);
  RT::Tuple expected = RT::Tuple(1, 2, 3, 4);
  REQUIRE(RT::identityMatrix<4>() * a == expected);
}

TEST_CASE("Transposing a matrix", "[Matrix]") {
  RT::Matrix<4> a =
      RT::Matrix<4>({0, 9, 3, 0, 9, 8, 0, 8, 1, 8, 5, 3, 0, 0, 5, 8});
  RT::Matrix<4> expected =
      RT::Matrix<4>({0, 9, 1, 0, 9, 8, 8, 0, 3, 0, 5, 5, 0, 8, 3, 8});
  REQUIRE(a.transpose() == expected);
}

TEST_CASE("Transposing the identity matrix", "[Matrix]") {
  RT::Matrix<4> a = RT::identityMatrix<4>();
  RT::Matrix<4> expected = RT::identityMatrix<4>();
  REQUIRE(a.transpose() == expected);
}

TEST_CASE("Calculating the determinant of a 2x2 matrix", "[Matrix]") {
  RT::Matrix<2> a = RT::Matrix<2>({1, 5, -3, 2});
  REQUIRE(a.determinant() == 17);
}

TEST_CASE("A submatrix of a 3x3 matrix is a 2x2 matrix", "[Matrix]") {
  RT::Matrix<3> a = RT::Matrix<3>({1, 5, 0, -3, 2, 7, 0, 6, -3});
  RT::Matrix<2> expected = RT::Matrix<2>({-3, 2, 0, 6});
  REQUIRE(a.submatrix(0, 2) == expected);
}

TEST_CASE("A submatrix of a 4x4 matrix is a 3x3 matrix", "[Matrix]") {
  RT::Matrix<4> a =
      RT::Matrix<4>({-6, 1, 1, 6, -8, 5, 8, 6, -1, 0, 8, 2, -7, 1, -1, 1});
  RT::Matrix<3> expected = RT::Matrix<3>({-6, 1, 6, -8, 8, 6, -7, -1, 1});
  REQUIRE(a.submatrix(2, 1) == expected);
}

TEST_CASE("Calculating a minor of a 3x3 matrix", "[Matrix]") {
  RT::Matrix<3> a = RT::Matrix<3>({3, 5, 0, 2, -1, -7, 6, -1, 5});
  RT::Matrix<2> b = a.submatrix(1, 0);
  REQUIRE(b.determinant() == 25);
  REQUIRE(a.minor(1, 0) == 25);
}

TEST_CASE("Calculating a cofactor of a 3x3 matrix", "[Matrix]") {
  RT::Matrix<3> a = RT::Matrix<3>({3, 5, 0, 2, -1, -7, 6, -1, 5});
  REQUIRE(a.minor(0, 0) == -12);
  REQUIRE(a.cofactor(0, 0) == -12);
  REQUIRE(a.minor(1, 0) == 25);
  REQUIRE(a.cofactor(1, 0) == -25);
}

TEST_CASE("Calculating the determinant of a 3x3 matrix", "[Matrix]") {
  RT::Matrix<3> a = RT::Matrix<3>({1, 2, 6, -5, 8, -4, 2, 6, 4});
  REQUIRE(a.cofactor(0, 0) == 56);
  REQUIRE(a.cofactor(0, 1) == 12);
  REQUIRE(a.cofactor(0, 2) == -46);
  REQUIRE(a.determinant() == -196);
}

TEST_CASE("Calculating the determinant of a 4x4 matrix", "[Matrix]") {
  RT::Matrix<4> a =
      RT::Matrix<4>({-2, -8, 3, 5, -3, 1, 7, 3, 1, 2, -9, 6, -6, 7, 7, -9});
  REQUIRE(a.cofactor(0, 0) == 690);
  REQUIRE(a.cofactor(0, 1) == 447);
  REQUIRE(a.cofactor(0, 2) == 210);
  REQUIRE(a.cofactor(0, 3) == 51);
  REQUIRE(a.determinant() == -4071);
}

TEST_CASE("Testing an invertible matrix for invertibility", "[Matrix]") {
  RT::Matrix<4> a =
      RT::Matrix<4>({6, 4, 4, 4, 5, 5, 7, 6, 4, -9, 3, -7, 9, 1, 7, -6});
  REQUIRE(a.determinant() == -2120);
  REQUIRE(a.isInvertible());
}

TEST_CASE("Testing a noninvertible matrix for invertibility", "[Matrix]") {
  RT::Matrix<4> a =
      RT::Matrix<4>({-4, 2, -2, -3, 9, 6, 2, 6, 0, -5, 1, -5, 0, 0, 0, 0});
  REQUIRE(a.determinant() == 0);
  REQUIRE(!a.isInvertible());
}

TEST_CASE("Calculating the inverse of a matrix", "[Matrix]") {
  RT::Matrix<4> a =
      RT::Matrix<4>({-5, 2, 6, -8, 1, -5, 1, 8, 7, 7, -6, -7, 1, -3, 7, 4});
  RT::Matrix<4> b = a.inverse();
  REQUIRE(a.determinant() == 532);
  REQUIRE(a.cofactor(2, 3) == -160);
  REQUIRE(b(3, 2) == -160.0 / 532);
  REQUIRE(a.cofactor(3, 2) == 105);
  REQUIRE(b(2, 3) == 105.0 / 532);
  RT::Matrix<4> expected =
      RT::Matrix<4>({0.21805, 0.45113, 0.24060, -0.04511, -0.80827, -1.45677,
                     -0.44361, 0.52068, -0.07895, -0.22368, -0.05263, 0.19737,
                     -0.52256, -0.81391, -0.30075, 0.30639});

  REQUIRE(b == expected);
}

TEST_CASE("Calculating the inverse of another matrix", "[Matrix]") {
  RT::Matrix<4> a =
      RT::Matrix<4>({8, -5, 9, 2, 7, 5, 6, 1, -6, 0, 9, 6, -3, 0, -9, -4});
  RT::Matrix<4> expected =
      RT::Matrix<4>({-0.15385, -0.15385, -0.28205, -0.53846, -0.07692, 0.12308,
                     0.02564, 0.03077, 0.35897, 0.35897, 0.43590, 0.92308,
                     -0.69231, -0.69231, -0.76923, -1.92308});
  REQUIRE(a.inverse() == expected);
}

TEST_CASE("Calculating the inverse of a third matrix", "[Matrix]") {
  RT::Matrix<4> a =
      RT::Matrix<4>({9, 3, 0, 9, -5, -2, -6, -3, -4, 9, 6, 4, -7, 6, 6, 2});
  RT::Matrix<4> expected =
      RT::Matrix<4>({-0.04074, -0.07778, 0.14444, -0.22222, -0.07778, 0.03333,
                     0.36667, -0.33333, -0.02901, -0.14630, -0.10926, 0.12963,
                     0.17778, 0.06667, -0.26667, 0.33333});
  REQUIRE(a.inverse() == expected);
}

TEST_CASE("Multiplying a product by its inverse", "[Matrix]") {
  RT::Matrix<4> a =
      RT::Matrix<4>({3, -9, 7, 3, 3, -8, 2, -9, -4, 4, 4, 1, -6, 5, -1, 1});
  RT::Matrix<4> b =
      RT::Matrix<4>({8, 2, 2, 2, 3, -1, 7, 0, 7, 0, 5, 4, 6, -2, 0, 5});
  RT::Matrix<4> c = a * b;
  REQUIRE(c * b.inverse() == a);
}

TEST_CASE("Multiplying by a translation matrix", "[Matrix]") {
  RT::Matrix<4> transform = RT::translation(5, -3, 2);
  RT::Tuple p = RT::point(-3, 4, 5);
  REQUIRE(transform * p == RT::point(2, 1, 7));
}

TEST_CASE("Multiplying by the inverse of a translation matrix", "[Matrix]") {
  RT::Matrix<4> transform = RT::translation(5, -3, 2);
  RT::Matrix<4> inv = transform.inverse();
  RT::Tuple p = RT::point(-3, 4, 5);
  REQUIRE(inv * p == RT::point(-8, 7, 3));
}

TEST_CASE("Translation does not affect vectors", "[Matrix]") {
  RT::Matrix<4> transform = RT::translation(5, -3, 2);
  RT::Tuple v = RT::vector(-3, 4, 5);
  REQUIRE(transform * v == v);
}

TEST_CASE("A scaling matrix applied to a point", "[Matrix]") {
  RT::Matrix<4> transform = RT::scaling(2, 3, 4);
  RT::Tuple p = RT::point(-4, 6, 8);
  REQUIRE(transform * p == RT::point(-8, 18, 32));
}

TEST_CASE("A scaling matrix applied to a vector", "[Matrix]") {
  RT::Matrix<4> transform = RT::scaling(2, 3, 4);
  RT::Tuple v = RT::vector(-4, 6, 8);
  REQUIRE(transform * v == RT::vector(-8, 18, 32));
}

TEST_CASE("Multiplying by the inverse of a scaling matrix", "[Matrix]") {
  RT::Matrix<4> transform = RT::scaling(2, 3, 4);
  RT::Matrix<4> inv = transform.inverse();
  RT::Tuple v = RT::vector(-4, 6, 8);
  REQUIRE(inv * v == RT::vector(-2, 2, 2));
}

TEST_CASE("Reflection is scaling by a negative value", "[Matrix]") {
  RT::Matrix<4> transform = RT::scaling(-1, 1, 1);
  RT::Tuple p = RT::point(2, 3, 4);
  REQUIRE(transform * p == RT::point(-2, 3, 4));
}

TEST_CASE("Rotating a point around the x axis", "[Matrix]") {
  RT::Tuple p = RT::point(0, 1, 0);
  RT::Matrix<4> half_quarter = RT::rotationX(M_PI / 4);
  RT::Matrix<4> full_quarter = RT::rotationX(M_PI / 2);
  REQUIRE(half_quarter * p == RT::point(0, std::sqrt(2) / 2, std::sqrt(2) / 2));
  REQUIRE(full_quarter * p == RT::point(0, 0, 1));
}

TEST_CASE("The inverse of an x-rotation rotates in the opposite direction",
          "[Matrix]") {
  RT::Tuple p = RT::point(0, 1, 0);
  RT::Matrix<4> half_quarter = RT::rotationX(M_PI / 4);
  RT::Matrix<4> inv = half_quarter.inverse();
  REQUIRE(inv * p == RT::point(0, std::sqrt(2) / 2, -std::sqrt(2) / 2));
}

TEST_CASE("Rotating a point around the y axis", "[Matrix]") {
  RT::Tuple p = RT::point(0, 0, 1);
  RT::Matrix<4> half_quarter = RT::rotationY(M_PI / 4);
  RT::Matrix<4> full_quarter = RT::rotationY(M_PI / 2);
  REQUIRE(half_quarter * p == RT::point(std::sqrt(2) / 2, 0, std::sqrt(2) / 2));
  REQUIRE(full_quarter * p == RT::point(1, 0, 0));
}

TEST_CASE("Rotating a point around the z axis", "[Matrix]") {
  RT::Tuple p = RT::point(0, 1, 0);
  RT::Matrix<4> half_quarter = RT::rotationZ(M_PI / 4);
  RT::Matrix<4> full_quarter = RT::rotationZ(M_PI / 2);
  REQUIRE(half_quarter * p ==
          RT::point(-std::sqrt(2) / 2, std::sqrt(2) / 2, 0));
  REQUIRE(full_quarter * p == RT::point(-1, 0, 0));
}

TEST_CASE("A shearing transformation moves x in proportion to y", "[Matrix]") {
  RT::Matrix<4> transform = RT::shearing(1, 0, 0, 0, 0, 0);
  RT::Tuple p = RT::point(2, 3, 4);
  REQUIRE(transform * p == RT::point(5, 3, 4));
}

TEST_CASE("A shearing transformation moves x in proportion to z", "[Matrix]") {
  RT::Matrix<4> transform = RT::shearing(0, 1, 0, 0, 0, 0);
  RT::Tuple p = RT::point(2, 3, 4);
  REQUIRE(transform * p == RT::point(6, 3, 4));
}

TEST_CASE("A shearing transformation moves y in proportion to x", "[Matrix]") {
  RT::Matrix<4> transform = RT::shearing(0, 0, 1, 0, 0, 0);
  RT::Tuple p = RT::point(2, 3, 4);
  REQUIRE(transform * p == RT::point(2, 5, 4));
}

TEST_CASE("A shearing transformation moves y in proportion to z", "[Matrix]") {
  RT::Matrix<4> transform = RT::shearing(0, 0, 0, 1, 0, 0);
  RT::Tuple p = RT::point(2, 3, 4);
  REQUIRE(transform * p == RT::point(2, 7, 4));
}

TEST_CASE("A shearing transformation moves z in proportion to x", "[Matrix]") {
  RT::Matrix<4> transform = RT::shearing(0, 0, 0, 0, 1, 0);
  RT::Tuple p = RT::point(2, 3, 4);
  REQUIRE(transform * p == RT::point(2, 3, 6));
}

TEST_CASE("A shearing transformation moves z in proportion to y", "[Matrix]") {
  RT::Matrix<4> transform = RT::shearing(0, 0, 0, 0, 0, 1);
  RT::Tuple p = RT::point(2, 3, 4);
  REQUIRE(transform * p == RT::point(2, 3, 7));
}

TEST_CASE("Individual transformations are applied in sequence", "[Matrix]") {
  RT::Tuple p = RT::point(1, 0, 1);
  RT::Matrix<4> A = RT::rotationX(M_PI / 2);
  RT::Matrix<4> B = RT::scaling(5, 5, 5);
  RT::Matrix<4> C = RT::translation(10, 5, 7);
  RT::Tuple p2 = A * p;
  REQUIRE(p2 == RT::point(1, -1, 0));
  RT::Tuple p3 = B * p2;
  REQUIRE(p3 == RT::point(5, -5, 0));
  RT::Tuple p4 = C * p3;
  REQUIRE(p4 == RT::point(15, 0, 7));
}

TEST_CASE("Chained transformations must be applied in reverse order",
          "[Matrix]") {
  RT::Tuple p = RT::point(1, 0, 1);
  RT::Matrix<4> A = RT::rotationX(M_PI / 2);
  RT::Matrix<4> B = RT::scaling(5, 5, 5);
  RT::Matrix<4> C = RT::translation(10, 5, 7);
  RT::Matrix<4> T = C * B * A;
  REQUIRE(T * p == RT::point(15, 0, 7));
  REQUIRE((p >>= A >>= B >>= C) == RT::point(15, 0, 7));
}