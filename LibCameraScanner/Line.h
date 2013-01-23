#pragma once
#include <complex>
namespace Line
{
	typedef std::complex<double> P;

	// 許容する誤差ε
#define EPS (1e-10)
	// 2つのスカラーが等しいかどうか
#define EQ(a,b) (abs((a)-(b)) < EPS)
	// 2つのベクトルが等しいかどうか
#define EQV(a,b) ( EQ((a).real(), (b).real()) && EQ((a).imag(), (b).imag()) )

	// ベクトルaの絶対値を求める
	double length(const P& a);

	// 2点a,b間の距離を求める
	double distance(const P& a, const P& b);

	// ベクトルaの単位ベクトルを求める
	P unit_vector(const P& a);

	// ベクトルaの法線ベクトルn1,n2を求める
	std::pair<P, P> normal(const P& a);

	// ベクトルaの単位法線ベクトルun1,un2を求める
	std::pair<P, P> unit_normal(const P& a);

	// 内積 (dot product) : a・b = |a||b|cosΘ
	double dot(const P& a, const P& b);

	// 外積 (cross product) : a×b = |a||b|sinΘ
	double cross(const P& a, const P& b);

	// 2直線の直交判定 : a⊥b <=> dot(a, b) = 0
	bool is_orthogonal(const P& a1, const P& a2, const P& b1, const P& b2);

	// 2直線の平行判定 : a//b <=> cross(a, b) = 0
	bool is_parallel(const P& a1, const P& a2, const P& b1, const P& b2);

	// 点cが線分a,b上にあるかないか
	bool is_point_on_line(const P& a, const P& b, const P& c);

	// 点a,bを通る直線と点cとの距離
	double distance_l_p(const P& a, const P& b, const P& c);

	// 点a,bを端点とする線分と点cとの距離
	double distance_ls_p(const P& a, const P& b, const P& c);

	// a1,a2を端点とする線分とb1,b2を端点とする線分の交差判定
	bool is_intersected_ls(const P& a1, const P& a2, const P& b1, const P& b2);

	// a1,a2を端点とする線分とb1,b2を端点とする線分の交点計算
	P intersection_ls(const P& a1, const P& a2, const P& b1, const P& b2);

	// a1,a2を通る直線とb1,b2を通る直線の交差判定
	bool is_intersected_l(const P& a1, const P& a2, const P& b1, const P& b2);

	// a1,a2を通る直線とb1,b2を通る直線の交点計算
	P intersection_l(const P& a1, const P& a2, const P& b1, const P& b2);
}
