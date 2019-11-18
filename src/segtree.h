#include <vector>
#include <functional>
#include <limits>
#include <cassert>
#include <iostream>

template<typename T>
class SegmentTree {
  using F = std::function<T(T, T)>;
  std::vector<T> data;
  int n = 1;
  F f;
  T ti;
public:
  SegmentTree(std::vector<T> data, T ti, F f);
  void SetVal(int idx, T x);
  T Query(int l, int r);
  void PrintData();
};

template<typename T>
SegmentTree<T>::SegmentTree(std::vector<T> d, T ti, F f) :
ti(ti), f(f) {
  while (n < d.size()) {
    n <<= 1;
  }
  data.resize(2*n, ti);
  for (int i = 0; i < d.size(); i++) {
    data[n+i] = d[i];
  }
  for (int i = n-1; i > 0; i--) {
    data[i] = f(data[i<<1], data[(i<<1)+1]);
  }
}

template<typename T>
void SegmentTree<T>::SetVal(int idx, T x) {
  assert((0 <= idx) && (idx < n));
  data[n+idx] = x;
  for (int i = ((n+idx)>>1); i > 0; i>>=1) {
    data[i] = f(data[i<<1], data[(i<<1)+1]);
  }
}

template<typename T>
T SegmentTree<T>::Query(int l, int r) {
  assert(
      ((0 <= l) && (l < n)) &&
      ((0 <= r) && (r < n)) &&
      (l < r));
  l += n;
  r += n-1;
  T vl = ti;
  T vr = ti;
  for (; l<=r; l>>=1, r>>=1) {
    if (l&1) {
      vl = f(vl, data[l]);
      l++;
    }
    if (!(r&1)) {
      vr = f(vr, data[r]);
      r--;
    }
  }
  return f(vl, vr);
}

template<typename T>
void SegmentTree<T>::PrintData() {
  for (int i = 0; i < data.size(); i++) {
    std::cout << " " << data[i];
  }
  std::cout << std::endl;
}


template<typename T, typename E>
class LazySegmentTree {
  using F = std::function<T(T, T)>;
  using G = std::function<T(T, E)>;
  using H = std::function<E(E, E)>;
  std::vector<T> data;
  std::vector<E> lazy;
  int n = 1;
  F f;
  G g;
  H h;
  T ti;
  E ei;
  void Operate(int idx);
  void Propagate(int idx);
  void PropagateAndOperate(int idx);
  void LazyPropagateFromRoot(int idx);
  void DataPropagateFromLeaf(int idx);
public:
  LazySegmentTree(std::vector<T> d, T ti, E ei, F f, G g, H h);
  void SetVal(int idx, T x);
  void Update(int l, int r, E x);
  T Query(int l, int r);
  void PrintData();
};

template<typename T, typename E>
LazySegmentTree<T, E>::LazySegmentTree(std::vector<T> d, T ti, E ei, F f, G g, H h) :
ti(ti), ei(ei), f(f), g(g), h(h) {
  while (n < d.size()) {
    n <<= 1;
  }
  data.resize(2*n, ti);
  lazy.resize(2*n, ei);
  for (int i = 0; i < d.size(); i++) {
    data[n+i] = d[i];
  }
  for (int i = n-1; i > 0; i--) {
    data[i] = f(data[i<<1], data[(i<<1)+1]);
  }
}

template<typename T, typename E>
void LazySegmentTree<T, E>::SetVal(int idx, T x) {
  assert((0 <= idx) && (idx < n));
  data[n+idx] = x;
  LazyPropagateFromRoot(n+idx);
  DataPropagateFromLeaf(n+idx);
}

template<typename T, typename E>
void LazySegmentTree<T, E>::Update(int l, int r, E x) {
  assert(
      ((0 <= l) && (l < n)) &&
      ((0 < r) && (r <= n)) &&
      (l < r));
  int l_, r_;
  l += n;
  r += n-1;
  l_ = l; r_ = r;

  LazyPropagateFromRoot(l_);
  LazyPropagateFromRoot(r_);

  for (; l<=r; l>>=1, r>>=1) {
    if (l&1) {
      lazy[l] = h(lazy[l], x);
      l++;
    }
    if (!(r&1)) {
      lazy[r] = h(lazy[r], x);
      r--;
    }
  }

  DataPropagateFromLeaf(l_);
  DataPropagateFromLeaf(r_);
}

template<typename T, typename E>
T LazySegmentTree<T, E>::Query(int l, int r) {
  assert(
      ((0 <= l) && (l < n)) &&
      ((0 < r) && (r <= n)) &&
      (l < r));
  l += n;
  r += n-1;
  T vl = ti;
  T vr = ti;

  LazyPropagateFromRoot(l);
  LazyPropagateFromRoot(r);

  for (; l<=r; l>>=1, r>>=1) {
    if (l&1) {
      vl = f(vl, data[l]);
      l++;
    }
    if (!(r&1)) {
      vr = f(vr, data[r]);
      r--;
    }
  }
  return f(vl, vr);
}

template<typename T, typename E>
void LazySegmentTree<T, E>::PrintData() {
  for (int i = 0; i < data.size(); i++) {
    std::cout << " " << data[i];
  }
  std::cout << std::endl;
  for (int i = 0; i < lazy.size(); i++) {
    std::cout << " " << lazy[i];
  }
  std::cout << std::endl;
}

template<typename T, typename E>
void LazySegmentTree<T, E>::Operate(int idx) {
  data[idx] = g(data[idx], lazy[idx]);
  lazy[idx] = ei;
}

template<typename T, typename E>
void LazySegmentTree<T, E>::Propagate(int idx) {
  if (idx >= n) return;
  lazy[(idx<<1)] = h(lazy[(idx<<1)], lazy[idx]);
  lazy[(idx<<1)+1] = h(lazy[(idx<<1)+1], lazy[idx]);
}

template<typename T, typename E>
void LazySegmentTree<T, E>::PropagateAndOperate(int idx) {
  Propagate(idx);
  Operate(idx);
}

template<typename T, typename E>
void LazySegmentTree<T, E>::LazyPropagateFromRoot(int idx) {
  PropagateAndOperate(1);
  for (int i = std::numeric_limits<int>::digits; i > 0; i--) {
    if ((idx>>i) == 0) continue;
    PropagateAndOperate(idx>>(i-1));
    PropagateAndOperate((idx>>(i-1))^1);
  }
}

template<typename T, typename E>
void LazySegmentTree<T, E>::DataPropagateFromLeaf(int idx) {
  for (; idx >= 1 ; idx>>=1) {
    if (idx != 1) {
      data[idx>>1] = f(g(data[idx], lazy[idx]), g(data[idx^1], lazy[idx^1]));
    }
  }
}
