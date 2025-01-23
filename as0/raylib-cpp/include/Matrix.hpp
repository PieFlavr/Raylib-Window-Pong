#ifndef RAYLIB_CPP_INCLUDE_MATRIX_HPP_
#define RAYLIB_CPP_INCLUDE_MATRIX_HPP_

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "./raymath.hpp"
#include "./RadiansDegrees.hpp"
#include "./Vector4.hpp"

#ifndef RAYLIB_CPP_NO_MATH
#include <cmath>
#endif

namespace raylib {
/**
 * Matrix type (OpenGL style 4x4 - right handed, column major)
 */
class Matrix : public ::Matrix {
 public:
    struct LocalSpace_t {};
    constexpr static LocalSpace_t LocalSpace{};
    struct GlobalSpace_t {};
    constexpr static GlobalSpace_t GlobalSpace{};

    Matrix(const ::Matrix& mat) : ::Matrix{
            mat.m0, mat.m4, mat.m8, mat.m12,
            mat.m1, mat.m5, mat.m9, mat.m13,
            mat.m2, mat.m6, mat.m10, mat.m14,
            mat.m3, mat.m7, mat.m11, mat.m15} {
        // Nothing.
    }

    Matrix(
            float m0 = 0, float m4 = 0, float m8 = 0, float m12 = 0,
            float m1 = 0, float m5 = 0, float m9 = 0, float m13 = 0,
            float m2 = 0, float m6 = 0, float m10 = 0, float m14 = 0,
            float m3 = 0, float m7 = 0, float m11 = 0, float m15 = 0) :
        ::Matrix{
            m0, m4, m8, m12,
            m1, m5, m9, m13,
            m2, m6, m10, m14,
            m3, m7, m11, m15} {
        // Nothing.
    }

    GETTERSETTER(float, M0, m0)
    GETTERSETTER(float, M1, m1)
    GETTERSETTER(float, M2, m2)
    GETTERSETTER(float, M3, m3)
    GETTERSETTER(float, M4, m4)
    GETTERSETTER(float, M5, m5)
    GETTERSETTER(float, M6, m6)
    GETTERSETTER(float, M7, m7)
    GETTERSETTER(float, M8, m8)
    GETTERSETTER(float, M9, m9)
    GETTERSETTER(float, M10, m10)
    GETTERSETTER(float, M11, m11)
    GETTERSETTER(float, M12, m12)
    GETTERSETTER(float, M13, m13)
    GETTERSETTER(float, M14, m14)
    GETTERSETTER(float, M15, m15)

    Matrix& operator=(const ::Matrix& matrix) {
        set(matrix);
        return *this;
    }

    Matrix& operator=(const Matrix& matrix) {
        set(matrix);
        return *this;
    }

    bool operator==(const ::Matrix& other) {
        return m0 == other.m0
            && m1 == other.m1
            && m2 == other.m2
            && m3 == other.m3
            && m4 == other.m4
            && m5 == other.m5
            && m6 == other.m6
            && m7 == other.m7
            && m8 == other.m8
            && m9 == other.m9
            && m10 == other.m10
            && m11 == other.m11
            && m12 == other.m12
            && m13 == other.m13
            && m14 == other.m14
            && m15 == other.m15;
    }

    bool operator!=(const ::Matrix& other) {
        return !(*this == other);
    }

#ifndef RAYLIB_CPP_NO_MATH
    /**
     * Returns the trace of the matrix (sum of the values along the diagonal)
     */
    inline float Trace() const {
        return ::MatrixTrace(*this);
    }

    /**
     * Transposes provided matrix
     */
    inline Matrix Transpose() const {
        return ::MatrixTranspose(*this);
    }

    /**
     * Inverts provided matrix
    */
    inline Matrix Invert() const {
        return ::MatrixInvert(*this);
    }

    /**
     * Creates an identity matrix
    */
    static Matrix Identity() {
        return ::MatrixIdentity();
    }

    /**
     * Creates a matrix that when multiplied by the current matrix will translate back to the origin
     * (New function not provided by raylib)
    */
    inline Matrix TranslateToOrigin() const {
        return CreateTranslate(-m12, -m13, -m14);
    }

    /**
     * Elementwise matrix addition
    */
    inline Matrix Add(const ::Matrix& right) const {
        return ::MatrixAdd(*this, right);
    }

    /**
     * Elementwise matrix addition
    */
    inline Matrix operator+(const ::Matrix& matrix) const {
        return ::MatrixAdd(*this, matrix);
    }

    /**
     * Elementwise matrix subtraction
    */
    inline Matrix Subtract(const ::Matrix& right) const {
        return ::MatrixSubtract(*this, right);
    }

    /**
     * Elementwise matrix subtraction
    */
    inline Matrix operator-(const ::Matrix& matrix) const {
        return ::MatrixSubtract(*this, matrix);
    }

    /**
     * Matrix multiplication
    */
    inline Matrix Multiply(const ::Matrix& right) const {
        return ::MatrixMultiply(*this, right);
    }

    /**
     * Matrix multiplication
    */
    inline Matrix operator*(const ::Matrix& matrix) const {
        return ::MatrixMultiply(*this, matrix);
    }

    /**
     * Creates a translation matrix
    */
    static Matrix CreateTranslate(float x, float y, float z) {
        return ::MatrixTranslate(x, y, z);
    }

    /** 
     * Translates the current matrix in global space
    */
    inline Matrix Translate(GlobalSpace_t, float x, float y, float z) const {
        return (*this) * CreateTranslate(x, y, z);
    }

    /**
     * Translates the current matrix in local space
    */
    inline Matrix Translate(LocalSpace_t, float x, float y, float z) const {
        return CreateTranslate(x, y, z) * (*this);
    }

    /**
     * Translates the current matrix in global space
    */
    inline Matrix Translate(float x, float y, float z) const {
        return Translate(GlobalSpace, x, y, z);
    }

    /**
     * Creates a translation matrix
    */
    static Matrix CreateTranslate(Vector3 translation) {
        return ::MatrixTranslate(translation.x, translation.y, translation.z);
    }

    /** 
     * Translates the current matrix in global space
    */
    Matrix Translate(GlobalSpace_t, Vector3 translation) const {
        return (*this) * CreateTranslate(translation.x, translation.y, translation.z);
    }

    /**
     * Translates the current matrix in local space
    */
    Matrix Translate(LocalSpace_t, Vector3 translation) const {
        return CreateTranslate(translation.x, translation.y, translation.z) * (*this);
    }

    /**
     * Translates the current matrix in global space
    */
    inline Matrix Translate(Vector3 translation) const {
        return Translate(GlobalSpace, translation.x, translation.y, translation.z);
    }

    /**
     * Creates a rotation matrix around the given axis
    */
    static Matrix CreateRotate(Vector3 axis, Radian angle) {
        return ::MatrixRotate(axis, angle);
    }

    /**
     * Rotates the current matrix in global space around the given axis
    */
    Matrix Rotate(GlobalSpace_t, Vector3 axis, Radian angle) const {
        return (*this) * CreateRotate(axis, angle);
    }

    /**
     * Rotates the current matrix in local space around the given axis
    */
    Matrix Rotate(LocalSpace_t, Vector3 axis, Radian angle) const {
        auto toOrigin = TranslateToOrigin();
        return (*this) * toOrigin * CreateRotate(axis, angle) * toOrigin.Invert();
    }

    /**
     * Rotates the current matrix in local space around the given axis
    */
    inline Matrix Rotate(Vector3 axis, Radian angle) const {
        return Rotate(LocalSpace, axis, angle);
    }

    /**
     * Creates a rotation matrix around the given quaternion
    */
    static Matrix CreateRotate(Quaternion quat) {
        auto [axis, angle] = quat.ToAxisAngle();
        return CreateRotate(axis, angle);
    }

    /**
     * Rotates the current matrix in global space around the given quaternion
    */
    Matrix Rotate(GlobalSpace_t, Quaternion quat) const {
        return (*this) * CreateRotate(quat);
    }

    /**
     * Rotates the current matrix in local space around the given quaternion
    */
    Matrix Rotate(LocalSpace_t, Quaternion quat) const {
        auto toOrigin = TranslateToOrigin();
        return (*this) * toOrigin * CreateRotate(quat) * toOrigin.Invert();
    }

    /**
     * Rotates the current matrix in local space around the given quaternion
    */
    inline Matrix Rotate(Quaternion quat) const {
        return Rotate(LocalSpace, quat);
    }

    /**
     * Creates a rotation matrix using the provided euler angles
    */
    static Matrix CreateRotateXYZ(Vector3 angle) {
        return ::MatrixRotateXYZ(angle);
    }

    /**
     * Rotates the current matrix using the provided euler angles in global space
    */
    Matrix RotateXYZ(GlobalSpace_t, Vector3 angle) const {
        return (*this) * CreateRotateXYZ(angle);
    }

    /**
     * Rotates the current matrix using the provided euler angles in local space
    */
    Matrix RotateXYZ(LocalSpace_t, Vector3 angle) const {
        auto toOrigin = TranslateToOrigin();
        return (*this) * toOrigin * CreateRotateXYZ(angle) * toOrigin.Invert();
    }

    /**
     * Rotates the current matrix using the provided euler angles in local space
    */
    inline Matrix RotateXYZ(Vector3 angle) const {
        return RotateXYZ(LocalSpace, angle);
    }

    /**
     * Creates a rotation matrix using the provided euler angles
    */
    static Matrix CreateRotateXYZ(Radian x, Radian y, Radian z) {
        return CreateRotateXYZ(Vector3{x, y, z});
    }

    /**
     * Rotates the current matrix using the provided euler angles in global space
    */
    Matrix RotateXYZ(GlobalSpace_t, Radian x, Radian y, Radian z) const {
        return (*this) * CreateRotateXYZ(x, y, z);
    }

    /**
     * Rotates the current matrix using the provided euler angles in local space
    */
    Matrix RotateXYZ(LocalSpace_t, Radian x, Radian y, Radian z) const {
        auto toOrigin = TranslateToOrigin();
        return (*this) * toOrigin * CreateRotateXYZ(x, y, z) * toOrigin.Invert();
    }

    /**
     * Rotates the current matrix using the provided euler angles in local space
    */
    inline Matrix RotateXYZ(Radian x, Radian y, Radian z) const {
        return RotateXYZ(LocalSpace, x, y, z);
    }

    /**
     * Creates a rotation matrix around the X axis
    */
    static Matrix CreateRotateX(Radian angle) {
        return ::MatrixRotateX(angle);
    }

    /**
     * Rotates the current matrix in global space around the X axis
    */
    Matrix RotateX(GlobalSpace_t, Radian angle) const {
        return (*this) * CreateRotateX(angle);
    }

    /**
     * Rotates the current matrix in local space around the X axis
    */
    Matrix RotateX(LocalSpace_t, Radian angle) const {
        auto toOrigin = TranslateToOrigin();
        return (*this) * toOrigin * CreateRotateX(angle) * toOrigin.Invert();
    }

    /**
     * Rotates the current matrix in local space around the X axis
    */
    inline Matrix RotateX(Radian angle) const {
        return RotateX(LocalSpace, angle);
    }

    /**
     * Creates a rotation matrix around the Y axis
    */
    static Matrix CreateRotateY(Radian angle) {
        return ::MatrixRotateY(angle);
    }

    /**
     * Rotates the current matrix in global space around the Y axis
    */
    Matrix RotateY(GlobalSpace_t, Radian angle) const {
        return (*this) * CreateRotateY(angle);
    }

    /**
     * Rotates the current matrix in local space around the Y axis
    */
    Matrix RotateY(LocalSpace_t, Radian angle) const {
        auto toOrigin = TranslateToOrigin();
        return (*this) * toOrigin * CreateRotateY(angle) * toOrigin.Invert();
    }

    /**
     * Rotates the current matrix in local space around the Y axis
    */
    inline Matrix RotateY(Radian angle) const {
        return RotateY(LocalSpace, angle);
    }

    /**
     * Creates a rotation matrix around the given axis
    */
    static Matrix CreateRotateZ(Radian angle) {
        return ::MatrixRotateZ(angle);
    }

    /**
     * Rotates the current matrix in global space around the Z axis
    */
    Matrix RotateZ(GlobalSpace_t, Radian angle) const {
        return (*this) * CreateRotateZ(angle);
    }

    /**
     * Rotates the current matrix in local space around the Z axis
    */
    Matrix RotateZ(LocalSpace_t, Radian angle) const {
        auto toOrigin = TranslateToOrigin();
        return (*this) * toOrigin * CreateRotateZ(angle) * toOrigin.Invert();
    }

    /**
     * Rotates the current matrix in local space around the Z axis
    */
    inline Matrix RotateZ(Radian angle) const {
        return RotateZ(LocalSpace, angle);
    }

    /**
     * Creates a scale matrix
    */
    static Matrix CreateScale(float x, float y, float z) {
        return ::MatrixScale(x, y, z);
    }

    /**
     * Scales the current matrix in global space
    */
    Matrix Scale(GlobalSpace_t, float x, float y, float z) const {
        return (*this) * CreateScale(x, y, z);
    }
    inline Matrix Scale(GlobalSpace_t, float all) const {
        return Scale(GlobalSpace, all, all, all);
    }

    /**
     * Scales the current matrix in local space
    */
    Matrix Scale(LocalSpace_t, float x, float y, float z) const {
        auto toOrigin = TranslateToOrigin();
        return (*this) * toOrigin * CreateScale(x, y, z) * toOrigin.Invert();
    }
    inline Matrix Scale(LocalSpace_t, float all) const {
        return Scale(LocalSpace, all, all, all);
    }

    /**
     * Scales the current matrix in local space
    */
    inline Matrix Scale(float x, float y, float z) const {
        return Scale(LocalSpace, x, y, z);
    }
    inline Matrix Scale(float all) const {
        return Scale(LocalSpace, all);
    }

    static Matrix Frustum(double left, double right, double bottom, double top,
            double near, double far) {
        return ::MatrixFrustum(left, right, bottom, top, near, far);
    }

    static Matrix Perspective(double fovy, double aspect, double near, double far) {
        return ::MatrixPerspective(fovy, aspect, near, far);
    }

    static Matrix Ortho(double left, double right, double bottom, double top,
            double near, double far) {
        return ::MatrixOrtho(left, right, bottom, top, near, far);
    }

    static Matrix LookAt(Vector3 eye, Vector3 target, Vector3 up) {
        return ::MatrixLookAt(eye, target, up);
    }

    float16 ToFloatV() const {
        return ::MatrixToFloatV(*this);
    }

    operator float16() {
        return ToFloatV();
    }

    /**
     * Set shader uniform value (matrix 4x4)
     */
    Matrix& SetShaderValue(const ::Shader& shader, int uniformLoc) {
        ::SetShaderValueMatrix(shader, uniformLoc, *this);
        return *this;
    }

    static Matrix GetCamera(const ::Camera& camera) {
        return ::GetCameraMatrix(camera);
    }

    static Matrix GetCamera(const ::Camera2D& camera) {
        return ::GetCameraMatrix2D(camera);
    }

#endif

 protected:
    void set(const ::Matrix& mat) {
        m0 = mat.m0;
        m1 = mat.m1;
        m2 = mat.m2;
        m3 = mat.m3;
        m4 = mat.m4;
        m5 = mat.m5;
        m6 = mat.m6;
        m7 = mat.m7;
        m8 = mat.m8;
        m9 = mat.m9;
        m10 = mat.m10;
        m11 = mat.m11;
        m12 = mat.m12;
        m13 = mat.m13;
        m14 = mat.m14;
        m15 = mat.m15;
    }
};

using Transform = Matrix; // raylib::Transform is also a matrix
}  // namespace raylib

using RMatrix = raylib::Matrix;

#endif  // RAYLIB_CPP_INCLUDE_MATRIX_HPP_
