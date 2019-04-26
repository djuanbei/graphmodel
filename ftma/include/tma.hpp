/**
 * @file   TMA.hpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Mar 31 21:25:54 2019
 *
 * @brief  a model for timed automata
 *
 *
 */

#ifndef __TIMED_AUTOMATA_
#define __TIMED_AUTOMATA_
namespace ftma {

template <typename L, typename E> struct TMA {
public:
  vector<L> locations;
  vector<E> edges;
  int       initial_loc;
  int       clock_num;

  TMA( vector<L> &locs, vector<E> &es, int init, int vnum )
      : locations( locs )
      , edges( es ) {
    initial_loc = init;
    clock_num   = vnum;
  }
};

} // namespace ftma

#endif
