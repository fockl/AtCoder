#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<ll> vll;
typedef vector<bool> vb;
typedef vector<vector<ll>> vvll;
typedef vector<vector<vector<ll>>> vvvll;

#define FOR(i,x,y) for(ll i=(ll)x; i<(ll)y; ++i)
#define REP(i,y) FOR(i, 0, y)

constexpr int INF = (1<<30);
constexpr ll INFLL = 1LL<<62;
constexpr long double EPS = 1e-12;
constexpr ll MOD = (ll)((1E+9)+7);

void final_check(vll& d, vll& c, vvll& v, vll& others){
  bool repeat_flag=true;
  while(repeat_flag){
    ll new_index = others[0];
    ll N = others[1];
    vb flag(new_index, false);
    REP(i, d.size()){
      REP(j, d[i]){
        flag[v[i][j]] = true;
      }
    }
    ll output = -1;
    repeat_flag = false;
    FOR(i, N+1, new_index){
      if(!flag[i]){
        output = i;
        repeat_flag = true;
        break;
      }
    }
    if(repeat_flag){
      REP(i, d.size()){
        REP(j, d[i]){
          if(v[i][j]>output) v[i][j]--;
        }
      }
      others[0]--;
    }
  }
  return;
}

ll after_process(vll& d, vll& c, vvll& v){
  ll count = 0;

  REP(i, d.size()){
    if(d[i]<0) count++;
  }

  ll d0_init=-1;
  REP(i, d.size()){
    if(d[i]!=0) continue;
    if(d0_init<0){
      d0_init = i;
    }else{
      d[i] = -1;
      c[d0_init] += c[i];
      count++;
    }
  }

  if(d0_init>=0){
    if(c[d0_init]==0){
      d[d0_init]=-1;
      count++;
    }
  }

  map<ll, ll> mp1_after;
  REP(i, d.size()){
    if(d[i]!=1) continue;
    ll hash = v[i][0];
    if(mp1_after.find(hash)==mp1_after.end()){
      mp1_after[hash] = i;
    }else{
      d[i] = -1;
      c[mp1_after[hash]] += c[i];
      count++;
    }
  }
  for(auto iter=mp1_after.begin(); iter!=mp1_after.end(); ++iter){
    ll hash = iter->first;
    ll i = iter->second;
    if(c[i]==0){
      d[i] = -1;
      count++;
    }
  }

  map<ll, ll> mp2_after;
  REP(i, d.size()){
    if(d[i]!=2) continue;
    ll hash = v[i][0]*MOD+v[i][1];
    if(mp2_after.find(hash)==mp2_after.end()){
      mp2_after[hash] = i;
    }else{
      d[i] = -1;
      c[mp2_after[hash]] += c[i];
      count++;
    }
  }
  for(auto iter=mp2_after.begin(); iter!=mp2_after.end(); ++iter){
    ll hash = iter->first;
    ll i = iter->second;
    if(c[i]==0){
      d[i] = -1;
      count++;
    }
  }

  return count;
}

double calculate_cost(ll N, ll new_index, vll& d, vll& c, vvll& v){
  ll S = new_index-1;

  ll count = after_process(d, c, v);
  ll L = d.size()-count;
  ll max_c = 0;
  REP(i, d.size()){
    if(d[i]<0) continue;
    max_c = max(max_c, abs(c[i]));
    if(d[i]>2){
      S += (d[i]-1)/2;
      L += 4*(d[i]-2);
    }
  }
  if(S>3000) return -1.0;

  double PY = 1000.0/(5.0*(S-N)+L+1000.0);
  double PZ = 1000.0/((double)max_c+1000.0);
  double cost = PY*PZ;

  return cost;
}

void print_temp_result(ll N, vll& d, vll& c, vvll& v, ll new_index){
  ll count = after_process(d, c, v);
  //fprintf(stderr, "%ld %ld\n", new_index-1, d.size()-count);
  ll c_max = 0;
  REP(i, d.size()){
    c_max = max(c_max, abs(c[i]));
  }
  //fprintf(stderr, "C_MAX : %ld\n", c_max);
  //fprintf(stderr, "COST : %lf\n", calculate_cost(N, new_index, d, c, v));
}

void print_output(ll new_index, vll& d, vll& c, vvll& v){
  ll count = after_process(d, c, v);
  cout << new_index-1 << " " << d.size()-count << "\n";
  REP(i, d.size()){
    if(d[i]<0) continue;
    cout << d[i] << " " << c[i];
    REP(j, v[i].size()){
      cout << " " << v[i][j];
    }
    cout << "\n";
  }
  return;
}

ll check_max_coef_abs(vll& d, vll& c, vvll& v){
  ll ans = 0;
  map<ll, ll> mp;
  REP(i, d.size()){
    if(d[i]==0){
      mp[0] += c[i];
    }else if(d[i]==1){
      ll heap = v[i][0];
      mp[heap] += c[i];
    }else if(d[i]==2){
      ll heap = v[i][0]*MOD+v[i][1];
      mp[heap] += c[i];
    }
  }
  REP(i, d.size()){
    if(d[i]==0){
      continue;
    }else if(d[i]==1){
      ll heap = v[i][0];
      ans = max(ans, abs(mp[heap]));
    }else if(d[i]==2){
      ll heap = v[i][0]*MOD+v[i][1];
      ans = max(ans, abs(mp[heap]));
    }else{
      ans = max(ans, abs(c[i]));
    }
  }
  return ans;
}

ll Rosenberg_decomposition_process(vll& d, vll& c, vvll& v, ll new_index, ll first_index, ll second_index){
  if(first_index==second_index) return new_index;
  if(first_index>second_index) swap(first_index, second_index);

  ll c_posi_sum=0;
  ll c_nega_sum=0;

  REP(index, d.size()){
    ll d_tmp = d[index];
    if(d_tmp<3) continue;
    ll c_tmp = c[index];
    if(c_tmp==0) continue;
    vll v_tmp = v[index];

    bool first_flag = false;
    bool second_flag = false;

    REP(i, d_tmp){
      if(v_tmp[i]==first_index) first_flag = true;
      if(v_tmp[i]==second_index) second_flag = true;
    }

    if(!first_flag || !second_flag) continue;

    if(c[index]>0) c_posi_sum += abs(c[index]);
    if(c[index]<0) c_nega_sum += abs(c[index]);

    d[index] = d_tmp-1;
    v[index].resize(d_tmp-1);
    vll v_tmp1(d_tmp-1);
    ll shift=0;
    REP(i, d_tmp-2){
      if(v_tmp[i+shift]==first_index) shift++;
      if(v_tmp[i+shift]==second_index) shift++;
      v_tmp1[i] = v_tmp[i+shift];
    }
    v_tmp1[d_tmp-2] = new_index;
    v[index] = v_tmp1;
  }

  //fprintf(stderr, "c_sum = %d\n", c_sum);
  ll M = max(c_posi_sum, c_nega_sum);

  d.push_back(2);
  c.push_back(M);
  vll tmp2(2);
  tmp2[0] = first_index;
  tmp2[1] = second_index;
  v.push_back(tmp2);

  d.push_back(2);
  c.push_back(-2*M);
  vll tmp3(2);
  tmp3[0] = first_index;
  tmp3[1] = new_index;
  v.push_back(tmp3);

  d.push_back(2);
  c.push_back(-2*M);
  vll tmp4(2);
  tmp4[0] = second_index;
  tmp4[1] = new_index;
  v.push_back(tmp4);

  d.push_back(1);
  c.push_back(3*M);
  vll tmp5(1, new_index);
  v.push_back(tmp5);

  new_index++;

  return new_index;
}

ll positive_decomposition_process1(vll& d, vll& c, vvll& v, ll index, ll new_index, ll first, ll second){
  ll d_tmp = d[index];
  if(d_tmp<3) return new_index;
  ll c_tmp = c[index];
  vll v_tmp = v[index];

  d[index] = 2;
  v[index].resize(2);
  v[index][0] = v_tmp[first];
  v[index][1] = v_tmp[second];

  d.push_back(1);
  c.push_back(c_tmp);
  vll v_tmp1(1, new_index);
  v.push_back(v_tmp1);

  d.push_back(2);
  c.push_back(-c_tmp);
  vll v_tmp2(2);
  v_tmp2[0] = v_tmp[first];
  v_tmp2[1] = new_index;
  v.push_back(v_tmp2);

  d.push_back(2);
  c.push_back(-c_tmp);
  vll v_tmp3(2);
  v_tmp3[0] = v_tmp[second];
  v_tmp3[1] = new_index;
  v.push_back(v_tmp3);

  d.push_back(d_tmp-1);
  c.push_back(c_tmp);
  vll v_tmp4(d_tmp-1);
  ll shift=0;
  REP(j, d_tmp-2){
    if(j+shift==first) shift++;
    if(j+shift==second) shift++;
    v_tmp4[j] = v_tmp[j+shift];
  }
  v_tmp4[d_tmp-2] = new_index;
  v.push_back(v_tmp4);
  
  new_index++;

  return new_index;
}

ll positive_decomposition_process2(vll& d, vll& c, vvll& v, ll index, ll new_index){
  ll d_tmp = d[index];
  ll c_tmp = c[index];
  vll v_tmp = v[index];

  if(d_tmp<3) return new_index;
  if(c_tmp<=0) return new_index;

  d[index] = -1;

  REP(j1, d_tmp){
    FOR(j2, j1+1, d_tmp){
      d.push_back(2);
      c.push_back(c_tmp);
      vll v_tmp1(2);
      v_tmp1[0] = v_tmp[j1];
      v_tmp1[1] = v_tmp[j2];
      v.push_back(v_tmp1);
    }
  }

  ll nd = (d_tmp-1)/2;
  FOR(i, 1, nd+1){
    ll coef = 2;
    if(d_tmp%2==1 && i==nd) coef = 1;
    REP(j, d_tmp){
      d.push_back(2);
      c.push_back(-c_tmp*coef);
      vll v_tmp1(2);
      v_tmp1[0] = v_tmp[j];
      v_tmp1[1] = new_index;
      v.push_back(v_tmp1);
    }

    d.push_back(1);
    c.push_back(c_tmp*(coef*2*i-1));
    vll v_tmp1(1, new_index);
    v.push_back(v_tmp1);

    new_index++;
  }

  return new_index;
}

ll negative_decomposition_process1(vll& d, vll& c, vvll& v, ll index,ll new_index){
  ll d_tmp = d[index];
  if(d_tmp<3) return new_index;
  ll c_tmp = c[index];
  vll v_tmp = v[index];

  d[index] = 1;
  c[index] = -c_tmp*(d_tmp-1);
  v[index].resize(1);
  v[index][0] = new_index;

  REP(j, d_tmp){
    d.push_back(2);
    c.push_back(c_tmp);
    vll v_tmp1(2);
    v_tmp1[0] = v_tmp[j];
    v_tmp1[1] = new_index;
    v.push_back(v_tmp1);
  }

  new_index++;

  return new_index;
}

ll negative_decomposition_process2(vll& d, vll& c, vvll& v, ll index,ll new_index, ll first, ll second){
  ll d_tmp = d[index];
  if(d_tmp<3) return new_index;
  ll c_tmp = c[index];
  vll v_tmp = v[index];

  d[index] = 1;
  c[index] = -c_tmp*(3-1);
  v[index].resize(1);
  v[index][0] = new_index;

  d.push_back(2);
  c.push_back(c_tmp);
  vll v_tmp1(2);
  v_tmp1[0] = v_tmp[first];
  v_tmp1[1] = new_index;
  v.push_back(v_tmp1);

  d.push_back(2);
  c.push_back(c_tmp);
  vll v_tmp2(2);
  v_tmp2[0] = v_tmp[second];
  v_tmp2[1] = new_index;
  v.push_back(v_tmp2);

  d.push_back(d_tmp-1);
  c.push_back(c_tmp);
  vll v_tmp3(d_tmp-1);
  ll shift=0;
  REP(j, d_tmp-2){
    if(j+shift==first) shift++;
    if(j+shift==second) shift++;
    v_tmp3[j] = v_tmp[j+shift];
  }
  v_tmp3[d_tmp-2] = new_index;
  v.push_back(v_tmp3);

  new_index++;

  return new_index;
}

ll decomposition_process4(vll& d, vll& c, vvll& v, ll N){
  ll d_max=0;
  REP(i, d.size()){
    d_max = max(d_max, d[i]);
  }

  ll new_index = N+1;
  while(d_max>2){
    // map initialize
    map<ll, ll> index2_map;
    REP(i1, d.size()){
      if(d[i1]<3) continue;
      if(c[i1]==0) continue;
      REP(i2, d[i1]){
        ll s = v[i1][i2];
        FOR(i3, i2+1, d[i1]){
          ll e = v[i1][i3];
          ll hash = s*MOD+e;
          index2_map[hash]++;
        }
      }
    }

    after_process(d, c, v);

    ll first=1, second=2;
    ll map_max=0;
    for(auto iter=index2_map.begin(); iter!=index2_map.end(); ++iter){
      ll hash = iter->first;
      ll value = iter->second;
      if(value > map_max){
        first = hash/MOD;
        second = hash%MOD;
        map_max = value;
      }
    }
    if(map_max<2) break;
    new_index = Rosenberg_decomposition_process(d, c, v, new_index, first, second);

    d_max=0;
    REP(i, d.size()){
      d_max = max(d_max, d[i]);
    }

  }

  while(d_max>2){
    REP(i, d.size()){
      if(d[i]<3) continue;

      if(c[i] > 0){
        new_index = positive_decomposition_process2(d, c, v, i, new_index);
      }else{
        new_index = negative_decomposition_process1(d, c, v, i, new_index);
      }
    }
    d_max=0;
    REP(i, d.size()){
      d_max = max(d_max, d[i]);
    }
  }

  return new_index;
}

int main(){
  cin.tie(0); // cut the cin and cout (default, std::flush is performed after std::cin)
  ios::sync_with_stdio(false); // cut the iostream and stdio (DON'T endl; BUT "\n";)

  ll N, K;
  cin >> N >> K;
  vll d(K), c(K);
  vvll v(K);
  REP(i, K){
    cin >> d[i] >> c[i];
    v[i].resize(d[i]);
    REP(j, d[i]){
      cin >> v[i][j];
    }
  }

  vll d_ans = d;
  vll c_ans = c;
  vvll v_ans = v;
  ll new_index_ans = N+1;
  double point_ans = 0.0;

  ll new_index = decomposition_process4(d, c, v, N);

  vll others(2);
  others[0] = new_index;
  others[1] = N;
  final_check(d, c, v, others);
  new_index = others[0];

  print_output(new_index, d, c, v);

  return 0;
}
