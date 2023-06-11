// Segment Tree data structure
#include <vector>
#include <limits>

template<typename T, typename Compare = std::less<T>>
struct STComparator {
  Compare comp;
  T operator() (T a, T b) {
    return comp(a, b) ? a : b;
  }
};

// [l, r)
template<typename T = int, typename Transform = STComparator<T>, T DefaultVal = std::numeric_limits<T>::max()>
class SegmentTree {
  private:
  std::vector<T> tree;
  int size;
  Transform transform;
  T default_value = DefaultVal;

  inline int left_child(int x) {
    return 2 * x + 1;
  }

  inline int right_child(int x) {
    return 2 * x + 2;
  }  

  inline T conquer(T lhs, T rhs) {
    return transform(lhs, rhs);
  }

  /*
    l and r represents range you are looking for
    lx and rx represents the range that the current node x represents
  */
  T query(int l, int r, int x, int lx, int rx) {
    // range of l and r outside that of lx and rx
    if (lx >= r || l >= rx) {
      return default_value;
    }
    // range of lx and rx within l and r
    if (lx >= l && rx <= r) {
      return tree[x];
    }
    int midpoint = lx + (rx - lx) / 2;
    T left_chosen = query(l, r, left_child(x), lx, midpoint);
    T right_chosen = query(l, r, right_child(x), midpoint, rx);
    return conquer(left_chosen, right_chosen);
  }

  void update(int i, T v, int x, int lx, int rx) {
    // reached leaf node
    if (rx - lx == 1) {
      tree[x] = v;
      return;
    }

    int midpoint = lx + (rx - lx) / 2;
    if (i < midpoint) {
      update(i, v, left_child(x), lx, midpoint);
    } else {
      update(i, v, right_child(x), midpoint, rx);
    }
    tree[x] = conquer(tree[left_child(x)], tree[right_child(x)]);
  }

  void build(std::vector<T>& input, int x, int lx, int rx) {
    if (rx - lx == 1) {
      if (lx < input.size()) tree[x] = input[lx];
      return;
    }
    int m = lx + (rx - lx) / 2;
    build(input, left_child(x), lx, m);
    build(input, right_child(x), m, rx);
    tree[x] = conquer(tree[left_child(x)], tree[right_child(x)]);
  }

  public:
  using value_type = T;
  SegmentTree(size_t n, const Transform& transform = Transform()) : transform(transform){
    size = 1;
    while (size < n) size *= 2;
    tree.assign(2 * size - 1, default_value);
  }

  // build a segment tree using a vector
  void build(std::vector<T>& a) {
    build(a, 0, 0, size);
  }

  // range query for segment tree
  // inclusive l, exclusive r
  T query(int left, int right) {
    return query(left, right, 0, 0, size);
  }

  // point update for segment tree
  void update(int idx, T value) {
    update(idx, value, 0, 0, size);
  }
};

int demonstrate() {
  std::vector<int> a{ 56,34,12,664,53,65 };
  // MIN TREE
  SegmentTree min_tree(a.size());
  min_tree.build(a);
  std::cout << min_tree.query(3, 7) << std::endl;

  // MAX TREE
  SegmentTree<int, STComparator<int, std::greater<int>>, INT_MIN> max_tree(a.size());
  max_tree.build(a);
  std::cout << max_tree.query(3, 7) << std::endl;

  // SUMMATION TREE
  auto adder = [](auto a, auto b) { return a + b; };
  SegmentTree<int, decltype(adder), 0> sum_tree(a.size(), adder);
  sum_tree.build(a);
  std::cout << sum_tree.query(3, 7) << std::endl;

  return 0;
}