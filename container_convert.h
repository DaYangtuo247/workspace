//------------------------------------------------------
// std::vector<std::pair<int ,int >*>
struct pair_int_int {
  int first;
  int second;
};
struct vec_pair_int_int_p {
  pair_int_int* *vector_begin_0x0;
  pair_int_int* *vector_end_0x8;
  pair_int_int* *vector_cap_end_0x10;
};

//------------------------------------------------------
// std::tuple<HSTVector<rt::HARTRouteNetWrapper*>, std::shared_ptr<unsigned int>, std::map<double, int>, int>
struct vec_HARTRouteNetWrapper_p {
  rt::HARTRouteNetWrapper* *vector_begin_0x0;
  rt::HARTRouteNetWrapper* *vector_end_0x8;
  rt::HARTRouteNetWrapper* *vector_cap_end_0x10;
};
struct HSTVector_vec_HARTRouteNetWrapper_p {
  vec_HARTRouteNetWrapper_p inherit__0x0;
};
struct sharedPtr_unsigned_int {
  unsigned int *ptr;
  std::_Sp_counted_ptr *count;
};
struct mapItr_double_int {
  uint64_t header_color_0x0;
  mapItr_double_int *header_parent_0x8;
  mapItr_double_int *header_left_0x10;
  mapItr_double_int *header_right_0x18;
  double Key;
  int Value;
};
struct map_mapItr_double_int {
  uint64_t key_compare_0x0;
  mapItr_double_int end_address_0x8;
  mapItr_double_int *header_parent_0x10;
  mapItr_double_int *header_left_0x18;
  mapItr_double_int *header_right_0x20;
  uint64_t map_size_0x28;
};
struct tuple_HSTVector_vec_HARTRouteNetWrapper_p_sharedPtr_unsigned_int_map_mapItr_double_int_int {
  HSTVector_vec_HARTRouteNetWrapper_p Element_1;
  sharedPtr_unsigned_int Element_2;
  map_mapItr_double_int Element_3;
  int Element_4;
};

//------------------------------------------------------
// std::shared_ptr<int32_t>
struct sharedPtr_int32_t {
  int32_t *ptr;
  std::_Sp_counted_ptr *count;
};

//------------------------------------------------------
// HSTSet<unsigned int>
struct setItr_unsigned_int {
  uint64_t header_color_0x0;
  std::set *header_parent_0x8;
  std::set *header_left_0x10;
  std::set *header_right_0x18;
  unsigned int Key;
};
struct set_setItr_unsigned_int {
  uint64_t key_compare_0x0;
  setItr_unsigned_int end_address_0x8;
  setItr_unsigned_int *header_parent_0x10;
  setItr_unsigned_int *header_left_0x18;
  setItr_unsigned_int *header_right_0x20;
  uint64_t map_size_0x28;
};
struct HSTSet_set_setItr_unsigned_int {
  set_setItr_unsigned_int inherit__0x0;
};

//------------------------------------------------------
// std::vector<std::pair<unsigned int,rt::HDRTNetGuide*>>
struct pair_unsigned_int_HDRTNetGuide_p {
  unsigned int first;
  rt::HDRTNetGuide* second;
};
struct vec_pair_unsigned_int_HDRTNetGuide_p {
  pair_unsigned_int_HDRTNetGuide_p *vector_begin_0x0;
  pair_unsigned_int_HDRTNetGuide_p *vector_end_0x8;
  pair_unsigned_int_HDRTNetGuide_p *vector_cap_end_0x10;
};

//------------------------------------------------------
// std::unordered_map<std::map<double*, int>, HSTVector<rt::nodegraph::Node>>
struct mapItr_double_p_int {
  uint64_t header_color_0x0;
  mapItr_double_p_int *header_parent_0x8;
  mapItr_double_p_int *header_left_0x10;
  mapItr_double_p_int *header_right_0x18;
  double* Key;
  int Value;
};
struct map_mapItr_double_p_int {
  uint64_t key_compare_0x0;
  mapItr_double_p_int end_address_0x8;
  mapItr_double_p_int *header_parent_0x10;
  mapItr_double_p_int *header_left_0x18;
  mapItr_double_p_int *header_right_0x20;
  uint64_t map_size_0x28;
};
struct vec_Node {
  rt::nodegraph::Node *vector_begin_0x0;
  rt::nodegraph::Node *vector_end_0x8;
  rt::nodegraph::Node *vector_cap_end_0x10;
};
struct HSTVector_vec_Node {
  vec_Node inherit__0x0;
};
struct udmapItr_map_mapItr_double_p_int_HSTVector_vec_Node {
  udmapItr_map_mapItr_double_p_int_HSTVector_vec_Node *_M_nxt;
  map_mapItr_double_p_int Key;
  HSTVector_vec_Node Value;
};

//------------------------------------------------------
// HSTMap<HSTKeyValIter const &&,HSTKeyValIterLess>
struct mapItr_HSTKeyValIter_const_pp_HSTKeyValIterLess {
  uint64_t header_color_0x0;
  mapItr_HSTKeyValIter_const_pp_HSTKeyValIterLess *header_parent_0x8;
  mapItr_HSTKeyValIter_const_pp_HSTKeyValIterLess *header_left_0x10;
  mapItr_HSTKeyValIter_const_pp_HSTKeyValIterLess *header_right_0x18;
  HSTKeyValIter const ** Key;
  HSTKeyValIterLess Value;
};
struct map_mapItr_HSTKeyValIter_const_pp_HSTKeyValIterLess {
  uint64_t key_compare_0x0;
  mapItr_HSTKeyValIter_const_pp_HSTKeyValIterLess end_address_0x8;
  mapItr_HSTKeyValIter_const_pp_HSTKeyValIterLess *header_parent_0x10;
  mapItr_HSTKeyValIter_const_pp_HSTKeyValIterLess *header_left_0x18;
  mapItr_HSTKeyValIter_const_pp_HSTKeyValIterLess *header_right_0x20;
  uint64_t map_size_0x28;
};
struct HSTMap_map_mapItr_HSTKeyValIter_const_pp_HSTKeyValIterLess {
  map_mapItr_HSTKeyValIter_const_pp_HSTKeyValIterLess *inherit__0x0;
};

//------------------------------------------------------
// std::map<unsigned int, double>
struct mapItr_unsigned_int_double {
  uint64_t header_color_0x0;
  mapItr_unsigned_int_double *header_parent_0x8;
  mapItr_unsigned_int_double *header_left_0x10;
  mapItr_unsigned_int_double *header_right_0x18;
  unsigned int Key;
  double Value;
};
struct map_mapItr_unsigned_int_double {
  uint64_t key_compare_0x0;
  mapItr_unsigned_int_double end_address_0x8;
  mapItr_unsigned_int_double *header_parent_0x10;
  mapItr_unsigned_int_double *header_left_0x18;
  mapItr_unsigned_int_double *header_right_0x20;
  uint64_t map_size_0x28;
};

//------------------------------------------------------
// HSTVector<rt::nodegraph::Node*>
struct vec_Node_p {
  rt::nodegraph::Node* *vector_begin_0x0;
  rt::nodegraph::Node* *vector_end_0x8;
  rt::nodegraph::Node* *vector_cap_end_0x10;
};
struct HSTVector_vec_Node_p {
  vec_Node_p inherit__0x0;
};

//------------------------------------------------------
// HSTMap<HSTSet<HSTKeyValIter const &&>*,HSTKeyValIterLess>
struct setItr_HSTKeyValIter_const_pp {
  uint64_t header_color_0x0;
  std::set *header_parent_0x8;
  std::set *header_left_0x10;
  std::set *header_right_0x18;
  HSTKeyValIter const ** Key;
};
struct set_setItr_HSTKeyValIter_const_pp {
  uint64_t key_compare_0x0;
  setItr_HSTKeyValIter_const_pp end_address_0x8;
  setItr_HSTKeyValIter_const_pp *header_parent_0x10;
  setItr_HSTKeyValIter_const_pp *header_left_0x18;
  setItr_HSTKeyValIter_const_pp *header_right_0x20;
  uint64_t map_size_0x28;
};
struct HSTSet_set_setItr_HSTKeyValIter_const_pp {
  set_setItr_HSTKeyValIter_const_pp inherit__0x0;
};
struct mapItr_HSTSet_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess {
  uint64_t header_color_0x0;
  mapItr_HSTSet_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess *header_parent_0x8;
  mapItr_HSTSet_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess *header_left_0x10;
  mapItr_HSTSet_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess *header_right_0x18;
  HSTSet_set_setItr_HSTKeyValIter_const_pp* Key;
  HSTKeyValIterLess Value;
};
struct map_mapItr_HSTSet_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess {
  uint64_t key_compare_0x0;
  mapItr_HSTSet_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess end_address_0x8;
  mapItr_HSTSet_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess *header_parent_0x10;
  mapItr_HSTSet_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess *header_left_0x18;
  mapItr_HSTSet_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess *header_right_0x20;
  uint64_t map_size_0x28;
};
struct HSTMap_map_mapItr_HSTSet_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess {
  map_mapItr_HSTSet_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess *inherit__0x0;
};

//------------------------------------------------------
// std::vector<rt::nodegraph::Node*>
struct vec_Node_p {
  rt::nodegraph::Node* *vector_begin_0x0;
  rt::nodegraph::Node* *vector_end_0x8;
  rt::nodegraph::Node* *vector_cap_end_0x10;
};

//------------------------------------------------------
// std::vector<std::vector<rt::nodegraph::Node*>>
struct vec_Node_p {
  rt::nodegraph::Node* *vector_begin_0x0;
  rt::nodegraph::Node* *vector_end_0x8;
  rt::nodegraph::Node* *vector_cap_end_0x10;
};
struct vec_vec_Node_p {
  vec_Node_p *vector_begin_0x0;
  vec_Node_p *vector_end_0x8;
  vec_Node_p *vector_cap_end_0x10;
};

//------------------------------------------------------
// std::set<HSTKeyValIter const *,HSTKeyValIterLess>
struct setItr_HSTKeyValIter_const_p {
  uint64_t header_color_0x0;
  std::set *header_parent_0x8;
  std::set *header_left_0x10;
  std::set *header_right_0x18;
  HSTKeyValIter const * Key;
};
struct set_setItr_HSTKeyValIter_const_p {
  uint64_t key_compare_0x0;
  setItr_HSTKeyValIter_const_p end_address_0x8;
  setItr_HSTKeyValIter_const_p *header_parent_0x10;
  setItr_HSTKeyValIter_const_p *header_left_0x18;
  setItr_HSTKeyValIter_const_p *header_right_0x20;
  uint64_t map_size_0x28;
};

//------------------------------------------------------
// std::set<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>
struct setItr_HSTKeyValIter_const_pp {
  uint64_t header_color_0x0;
  std::set *header_parent_0x8;
  std::set *header_left_0x10;
  std::set *header_right_0x18;
  HSTKeyValIter const ** Key;
};
struct set_setItr_HSTKeyValIter_const_pp {
  uint64_t key_compare_0x0;
  setItr_HSTKeyValIter_const_pp end_address_0x8;
  setItr_HSTKeyValIter_const_pp *header_parent_0x10;
  setItr_HSTKeyValIter_const_pp *header_left_0x18;
  setItr_HSTKeyValIter_const_pp *header_right_0x20;
  uint64_t map_size_0x28;
};
struct setItr_set_setItr_HSTKeyValIter_const_pp_p {
  uint64_t header_color_0x0;
  std::set *header_parent_0x8;
  std::set *header_left_0x10;
  std::set *header_right_0x18;
  set_setItr_HSTKeyValIter_const_pp* Key;
};
struct set_setItr_set_setItr_HSTKeyValIter_const_pp_p {
  uint64_t key_compare_0x0;
  setItr_set_setItr_HSTKeyValIter_const_pp_p end_address_0x8;
  setItr_set_setItr_HSTKeyValIter_const_pp_p *header_parent_0x10;
  setItr_set_setItr_HSTKeyValIter_const_pp_p *header_left_0x18;
  setItr_set_setItr_HSTKeyValIter_const_pp_p *header_right_0x20;
  uint64_t map_size_0x28;
};

//------------------------------------------------------
// std::unordered_set<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>
struct setItr_HSTKeyValIter_const_pp {
  uint64_t header_color_0x0;
  std::set *header_parent_0x8;
  std::set *header_left_0x10;
  std::set *header_right_0x18;
  HSTKeyValIter const ** Key;
};
struct set_setItr_HSTKeyValIter_const_pp {
  uint64_t key_compare_0x0;
  setItr_HSTKeyValIter_const_pp end_address_0x8;
  setItr_HSTKeyValIter_const_pp *header_parent_0x10;
  setItr_HSTKeyValIter_const_pp *header_left_0x18;
  setItr_HSTKeyValIter_const_pp *header_right_0x20;
  uint64_t map_size_0x28;
};
struct udsetItr_set_setItr_HSTKeyValIter_const_pp_p {
  udsetItr_set_setItr_HSTKeyValIter_const_pp_p *_M_nxt;
  int64_t Key;
};

//------------------------------------------------------
// std::unordered_map<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>
struct setItr_HSTKeyValIter_const_pp {
  uint64_t header_color_0x0;
  std::set *header_parent_0x8;
  std::set *header_left_0x10;
  std::set *header_right_0x18;
  HSTKeyValIter const ** Key;
};
struct set_setItr_HSTKeyValIter_const_pp {
  uint64_t key_compare_0x0;
  setItr_HSTKeyValIter_const_pp end_address_0x8;
  setItr_HSTKeyValIter_const_pp *header_parent_0x10;
  setItr_HSTKeyValIter_const_pp *header_left_0x18;
  setItr_HSTKeyValIter_const_pp *header_right_0x20;
  uint64_t map_size_0x28;
};
struct udmapItr_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess {
  udmapItr_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess *_M_nxt;
  set_setItr_HSTKeyValIter_const_pp* Key;
  HSTKeyValIterLess Value;
};

//------------------------------------------------------
// std::map<std::set<HSTKeyValIter const &&>*,HSTKeyValIterLess>
struct setItr_HSTKeyValIter_const_pp {
  uint64_t header_color_0x0;
  std::set *header_parent_0x8;
  std::set *header_left_0x10;
  std::set *header_right_0x18;
  HSTKeyValIter const ** Key;
};
struct set_setItr_HSTKeyValIter_const_pp {
  uint64_t key_compare_0x0;
  setItr_HSTKeyValIter_const_pp end_address_0x8;
  setItr_HSTKeyValIter_const_pp *header_parent_0x10;
  setItr_HSTKeyValIter_const_pp *header_left_0x18;
  setItr_HSTKeyValIter_const_pp *header_right_0x20;
  uint64_t map_size_0x28;
};
struct mapItr_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess {
  uint64_t header_color_0x0;
  mapItr_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess *header_parent_0x8;
  mapItr_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess *header_left_0x10;
  mapItr_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess *header_right_0x18;
  set_setItr_HSTKeyValIter_const_pp* Key;
  HSTKeyValIterLess Value;
};
struct map_mapItr_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess {
  uint64_t key_compare_0x0;
  mapItr_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess end_address_0x8;
  mapItr_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess *header_parent_0x10;
  mapItr_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess *header_left_0x18;
  mapItr_set_setItr_HSTKeyValIter_const_pp_p_HSTKeyValIterLess *header_right_0x20;
  uint64_t map_size_0x28;
};

//------------------------------------------------------
// std::tuple<int,unsigned short,float>
struct tuple_int_unsigned_short_float {
  int Element_1;
  unsigned short Element_2;
  float Element_3;
};

//------------------------------------------------------
// std::tuple<std::set<std::map<short, int>>, std::string, int>
struct mapItr_short_int {
  uint64_t header_color_0x0;
  mapItr_short_int *header_parent_0x8;
  mapItr_short_int *header_left_0x10;
  mapItr_short_int *header_right_0x18;
  short Key;
  int Value;
};
struct map_mapItr_short_int {
  uint64_t key_compare_0x0;
  mapItr_short_int end_address_0x8;
  mapItr_short_int *header_parent_0x10;
  mapItr_short_int *header_left_0x18;
  mapItr_short_int *header_right_0x20;
  uint64_t map_size_0x28;
};
struct setItr_map_mapItr_short_int {
  uint64_t header_color_0x0;
  std::set *header_parent_0x8;
  std::set *header_left_0x10;
  std::set *header_right_0x18;
  map_mapItr_short_int Key;
};
struct set_setItr_map_mapItr_short_int {
  uint64_t key_compare_0x0;
  setItr_map_mapItr_short_int end_address_0x8;
  setItr_map_mapItr_short_int *header_parent_0x10;
  setItr_map_mapItr_short_int *header_left_0x18;
  setItr_map_mapItr_short_int *header_right_0x20;
  uint64_t map_size_0x28;
};
struct tuple_set_setItr_map_mapItr_short_int_string_int {
  set_setItr_map_mapItr_short_int Element_1;
  std::string Element_2;
  int Element_3;
};

//------------------------------------------------------
// std::map<std::tuple<std::set<std::map<short,int>>, std::string, int>,int,less>
struct mapItr_short_int {
  uint64_t header_color_0x0;
  mapItr_short_int *header_parent_0x8;
  mapItr_short_int *header_left_0x10;
  mapItr_short_int *header_right_0x18;
  short Key;
  int Value;
};
struct map_mapItr_short_int {
  uint64_t key_compare_0x0;
  mapItr_short_int end_address_0x8;
  mapItr_short_int *header_parent_0x10;
  mapItr_short_int *header_left_0x18;
  mapItr_short_int *header_right_0x20;
  uint64_t map_size_0x28;
};
struct setItr_map_mapItr_short_int {
  uint64_t header_color_0x0;
  std::set *header_parent_0x8;
  std::set *header_left_0x10;
  std::set *header_right_0x18;
  map_mapItr_short_int Key;
};
struct set_setItr_map_mapItr_short_int {
  uint64_t key_compare_0x0;
  setItr_map_mapItr_short_int end_address_0x8;
  setItr_map_mapItr_short_int *header_parent_0x10;
  setItr_map_mapItr_short_int *header_left_0x18;
  setItr_map_mapItr_short_int *header_right_0x20;
  uint64_t map_size_0x28;
};
struct tuple_set_setItr_map_mapItr_short_int_string_int {
  set_setItr_map_mapItr_short_int Element_1;
  std::string Element_2;
  int Element_3;
};
struct mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int {
  uint64_t header_color_0x0;
  mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int *header_parent_0x8;
  mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int *header_left_0x10;
  mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int *header_right_0x18;
  tuple_set_setItr_map_mapItr_short_int_string_int Key;
  int Value;
};
struct map_mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int {
  uint64_t key_compare_0x0;
  mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int end_address_0x8;
  mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int *header_parent_0x10;
  mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int *header_left_0x18;
  mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int *header_right_0x20;
  uint64_t map_size_0x28;
};

//------------------------------------------------------
// std::vector<HSTVector<rt::nodegraph::Node*>>
struct vec_Node_p {
  rt::nodegraph::Node* *vector_begin_0x0;
  rt::nodegraph::Node* *vector_end_0x8;
  rt::nodegraph::Node* *vector_cap_end_0x10;
};
struct HSTVector_vec_Node_p {
  vec_Node_p inherit__0x0;
};
struct vec_HSTVector_vec_Node_p {
  HSTVector_vec_Node_p *vector_begin_0x0;
  HSTVector_vec_Node_p *vector_end_0x8;
  HSTVector_vec_Node_p *vector_cap_end_0x10;
};

//------------------------------------------------------
// std::unordered_map<std::map<double, int>, HSTVector<rt::nodegraph::Node*>>
struct mapItr_double_int {
  uint64_t header_color_0x0;
  mapItr_double_int *header_parent_0x8;
  mapItr_double_int *header_left_0x10;
  mapItr_double_int *header_right_0x18;
  double Key;
  int Value;
};
struct map_mapItr_double_int {
  uint64_t key_compare_0x0;
  mapItr_double_int end_address_0x8;
  mapItr_double_int *header_parent_0x10;
  mapItr_double_int *header_left_0x18;
  mapItr_double_int *header_right_0x20;
  uint64_t map_size_0x28;
};
struct vec_Node_p {
  rt::nodegraph::Node* *vector_begin_0x0;
  rt::nodegraph::Node* *vector_end_0x8;
  rt::nodegraph::Node* *vector_cap_end_0x10;
};
struct HSTVector_vec_Node_p {
  vec_Node_p inherit__0x0;
};
struct udmapItr_map_mapItr_double_int_HSTVector_vec_Node_p {
  udmapItr_map_mapItr_double_int_HSTVector_vec_Node_p *_M_nxt;
  map_mapItr_double_int Key;
  HSTVector_vec_Node_p Value;
};

//------------------------------------------------------
// std::tuple<std::set<std::map<short, int>>, std::string, int>
struct mapItr_short_int {
  uint64_t header_color_0x0;
  mapItr_short_int *header_parent_0x8;
  mapItr_short_int *header_left_0x10;
  mapItr_short_int *header_right_0x18;
  short Key;
  int Value;
};
struct map_mapItr_short_int {
  uint64_t key_compare_0x0;
  mapItr_short_int end_address_0x8;
  mapItr_short_int *header_parent_0x10;
  mapItr_short_int *header_left_0x18;
  mapItr_short_int *header_right_0x20;
  uint64_t map_size_0x28;
};
struct setItr_map_mapItr_short_int {
  uint64_t header_color_0x0;
  std::set *header_parent_0x8;
  std::set *header_left_0x10;
  std::set *header_right_0x18;
  map_mapItr_short_int Key;
};
struct set_setItr_map_mapItr_short_int {
  uint64_t key_compare_0x0;
  setItr_map_mapItr_short_int end_address_0x8;
  setItr_map_mapItr_short_int *header_parent_0x10;
  setItr_map_mapItr_short_int *header_left_0x18;
  setItr_map_mapItr_short_int *header_right_0x20;
  uint64_t map_size_0x28;
};
struct tuple_set_setItr_map_mapItr_short_int_string_int {
  set_setItr_map_mapItr_short_int Element_1;
  std::string Element_2;
  int Element_3;
};

//------------------------------------------------------
// std::map<std::tuple<std::set<std::map<short,int>>,std::string,int>,int>
struct mapItr_short_int {
  uint64_t header_color_0x0;
  mapItr_short_int *header_parent_0x8;
  mapItr_short_int *header_left_0x10;
  mapItr_short_int *header_right_0x18;
  short Key;
  int Value;
};
struct map_mapItr_short_int {
  uint64_t key_compare_0x0;
  mapItr_short_int end_address_0x8;
  mapItr_short_int *header_parent_0x10;
  mapItr_short_int *header_left_0x18;
  mapItr_short_int *header_right_0x20;
  uint64_t map_size_0x28;
};
struct setItr_map_mapItr_short_int {
  uint64_t header_color_0x0;
  std::set *header_parent_0x8;
  std::set *header_left_0x10;
  std::set *header_right_0x18;
  map_mapItr_short_int Key;
};
struct set_setItr_map_mapItr_short_int {
  uint64_t key_compare_0x0;
  setItr_map_mapItr_short_int end_address_0x8;
  setItr_map_mapItr_short_int *header_parent_0x10;
  setItr_map_mapItr_short_int *header_left_0x18;
  setItr_map_mapItr_short_int *header_right_0x20;
  uint64_t map_size_0x28;
};
struct tuple_set_setItr_map_mapItr_short_int_string_int {
  set_setItr_map_mapItr_short_int Element_1;
  std::string Element_2;
  int Element_3;
};
struct mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int {
  uint64_t header_color_0x0;
  mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int *header_parent_0x8;
  mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int *header_left_0x10;
  mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int *header_right_0x18;
  tuple_set_setItr_map_mapItr_short_int_string_int Key;
  int Value;
};
struct map_mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int {
  uint64_t key_compare_0x0;
  mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int end_address_0x8;
  mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int *header_parent_0x10;
  mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int *header_left_0x18;
  mapItr_tuple_set_setItr_map_mapItr_short_int_string_int_int *header_right_0x20;
  uint64_t map_size_0x28;
};

//------------------------------------------------------
// std::tuple<unsigned int, int, double>
struct tuple_unsigned_int_int_double {
  unsigned int Element_1;
  int Element_2;
  double Element_3;
};

//------------------------------------------------------
// std::unordered_map<std::map<double, int>, HSTVector<rt::nodegraph::Node>>
struct mapItr_double_int {
  uint64_t header_color_0x0;
  mapItr_double_int *header_parent_0x8;
  mapItr_double_int *header_left_0x10;
  mapItr_double_int *header_right_0x18;
  double Key;
  int Value;
};
struct map_mapItr_double_int {
  uint64_t key_compare_0x0;
  mapItr_double_int end_address_0x8;
  mapItr_double_int *header_parent_0x10;
  mapItr_double_int *header_left_0x18;
  mapItr_double_int *header_right_0x20;
  uint64_t map_size_0x28;
};
struct vec_Node {
  rt::nodegraph::Node *vector_begin_0x0;
  rt::nodegraph::Node *vector_end_0x8;
  rt::nodegraph::Node *vector_cap_end_0x10;
};
struct HSTVector_vec_Node {
  vec_Node inherit__0x0;
};
struct udmapItr_map_mapItr_double_int_HSTVector_vec_Node {
  udmapItr_map_mapItr_double_int_HSTVector_vec_Node *_M_nxt;
  map_mapItr_double_int Key;
  HSTVector_vec_Node Value;
};

