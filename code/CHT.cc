struct Line {
  long long m, b;
  mutable function<const Line*()> succ;
  bool operator<(const Line& rhs) const{
    if(rhs.b!=-(1ll<<62)) return m>rhs.m; // < for max
    const Line* s = succ();
    if (!s) return 0;
    return b-s->b > (s->m -m)*rhs.m; // < for max
  }
};
struct HullDynamic : public multiset<Line> {
  bool bad(iterator y) {
    auto z = next(y);
    if(y==begin()){
      if(z==end())return 0;
      return y->m == z->m && y->b >= z->b; // <= for max
    }
    auto x = prev(y);
    if (z==end()) return y->m == x->m && y->b >= x->b;  // <= for max
    return (x->b - y->b)*1.0*(z->m - y->m) >= (y->b - z->b)*1.0*(y->m - x->m); 
  }
  void insert_line(long long m, long long b) {
    auto y = insert({ m, b });
    y->succ = [=]{return next(y)==end()? 0:&*next(y);};
    if(bad(y)) { erase(y); return; }
    while(next(y)!=end() && bad(next(y)))erase(next(y));
    while(y!=begin() && bad(prev(y)))erase(prev(y));
  }
  long long eval(long long x) {
    auto l = *lower_bound((Line){x,-(1ll<<62)});
    return l.m * x + l.b;
  }
};