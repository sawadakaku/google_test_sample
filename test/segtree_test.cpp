#include <gtest/gtest.h>
#include <segtree.h>
#include <functional>
#include <vector>
#include <limits>
#include <utility>

TEST(SEGTREE, rmq) {
  auto f_min = [](int a, int b) {return a<b ? a : b;};
  std::vector<int> v = {3, 4, 2, 0, 1, 9, 4, 8, 3, 3};

  auto s = SegmentTree<int>(v, std::numeric_limits<int>::max(), f_min);

  ASSERT_EQ(s.Query(0, 1), 3);
  ASSERT_EQ(s.Query(0, 3), 2);
  ASSERT_EQ(s.Query(2, 4), 0);
  ASSERT_EQ(s.Query(0, 9), 0);
  ASSERT_EQ(s.Query(4, 7), 1);
  ASSERT_EQ(s.Query(5, 8), 4);
  ASSERT_EQ(s.Query(5, 9), 3);


  s.SetVal(8, 5);
  ASSERT_EQ(s.Query(5, 9), 4);
  s.SetVal(6, 5);
  ASSERT_EQ(s.Query(5, 9), 5);
}

static int f_gcd(int a, int b) {
  if (a > b) std::swap(a, b);
  if (a == 0) return b;
  return f_gcd(b%a, a);
}

TEST(SEGTREE, gcd) {
  std::vector<int> v = {3, 4, 2, 0, 1, 9, 4, 8, 3, 3};

  auto s = SegmentTree<int>(v, 0, f_gcd);

  ASSERT_EQ(s.Query(0, 3), 1);
  ASSERT_EQ(s.Query(2, 4), 2);
  ASSERT_EQ(s.Query(0, 9), 1);
  ASSERT_EQ(s.Query(4, 7), 1);
  ASSERT_EQ(s.Query(5, 8), 1);
  ASSERT_EQ(s.Query(5, 9), 1);
}

static int f_lcm(int a, int b) {
  return a*b/f_gcd(a, b);
}

TEST(SEGTREE, lcm) {
  std::vector<int> v = {3, 4, 2, 1, 1, 9, 4, 8, 3, 3};

  auto s = SegmentTree<int>(v, 1, f_lcm);

  ASSERT_EQ(s.Query(0, 3), 12);
  ASSERT_EQ(s.Query(2, 4), 2);
  ASSERT_EQ(s.Query(0, 9), 72);
  ASSERT_EQ(s.Query(4, 7), 36);
  ASSERT_EQ(s.Query(5, 8), 72);
  ASSERT_EQ(s.Query(5, 9), 72);
}

TEST(LAZYSEGTREE, rmq) {
  auto f_min = [](int a, int b) {return a<b ? a : b;};
  auto g_min = [](int a, int b) {return (b==std::numeric_limits<int>::max()) ? a : b;};
  auto h_min = [](int a, int b) {return (b==std::numeric_limits<int>::max()) ? a : b;};
  std::vector<int> v = {3, 4, 2, 0, 1, 9, 4, 8, 3, 3};

  auto s = LazySegmentTree<int, int>(v, std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), f_min, g_min, h_min);

  ASSERT_EQ(s.Query(0, 1), 3);
  ASSERT_EQ(s.Query(0, 3), 2);
  ASSERT_EQ(s.Query(2, 4), 0);
  ASSERT_EQ(s.Query(0, 9), 0);
  ASSERT_EQ(s.Query(4, 7), 1);
  ASSERT_EQ(s.Query(5, 8), 4);
  ASSERT_EQ(s.Query(5, 9), 3);

  s.SetVal(8, 5);
  ASSERT_EQ(s.Query(5, 9), 4);
  s.SetVal(6, 5);
  ASSERT_EQ(s.Query(5, 9), 5);

  s.Update(6, 10, 6);
  ASSERT_EQ(s.Query(5, 9), 6);
  s.Update(3, 5, 3);
  ASSERT_EQ(s.Query(0, 10), 2);
}

struct T {
  int val, cnt;
  T(int val, int cnt): val(val), cnt(cnt) {};
  friend std::ostream& operator<<(std::ostream& os, const T& t) {
    os << t.val;
    return os;
  }
};

TEST(LAZYSEGTREE, rsq) {
  auto f_sum = [](T a, T b) {return T(a.val+b.val, a.cnt+b.cnt);};
  auto g_sum = [](T a, int b) {return T(a.val+a.cnt*b, a.cnt);};
  auto h_sum = [](int a, int b) {return a+b;};
  std::vector<int> v = {3, 4, 2, 0, 1, 9, 4, 8, 3, 3};
  std::vector<T> v_;
  for (int i = 0; i < v.size(); i++) {
    v_.emplace_back(v[i], 1);
  }

  auto s = LazySegmentTree<T, int>(v_, T(0, 1), 0, f_sum, g_sum, h_sum);

  ASSERT_EQ(s.Query(0, 1).val, 3);
  ASSERT_EQ(s.Query(0, 3).val, 9);
  ASSERT_EQ(s.Query(2, 4).val, 2);
  ASSERT_EQ(s.Query(0, 9).val, 34);
  ASSERT_EQ(s.Query(4, 7).val, 14);
  ASSERT_EQ(s.Query(5, 8).val, 21);
  ASSERT_EQ(s.Query(5, 9).val, 24);


  s.SetVal(8, T(5, 1));
  ASSERT_EQ(s.Query(5, 9).val, 26);
  s.SetVal(6, T(5, 1));
  ASSERT_EQ(s.Query(5, 9).val, 27);

  s.Update(6, 10, 6);
  ASSERT_EQ(s.Query(5, 9).val, 45);
  s.Update(3, 5, 3);
  ASSERT_EQ(s.Query(0, 10).val, 70);
}
