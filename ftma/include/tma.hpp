/**
 * @file   TA.hpp
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

template <typename L, typename T> class TA {
 
 private:
  vector<L> locations;
  vector<T> transitions;
  int       initial_loc;
  int       clock_num;
 public:
  TA( ){
    initial_loc=clock_num=-1;
  }
  TA( int init, int vnum ){
    initial_loc = init;
    clock_num   = vnum;
  }
  TA( vector<L> &locs, vector<T> &es, int init, int vnum )
      : locations( locs )
      , transitions( es ) {
    initial_loc = init;
    clock_num   = vnum;
  }


  void setClockNum( int num){
    clock_num=num;
  }
  
  int getClockNum( ) const{
    return clock_num;
  }


  void setInitialLoc(int loc) {
    initial_loc=loc;
  }
  int getInitialLoc( ) const{
    return  initial_loc;
  }


  const vector<T> & getTransitions(void ) const{
    return transitions;
  }

  const  vector<L> & getLocations( void) const{
    return locations;
  }
  /** 
   * Add one location to this timed automata
   * 
   * @param location 
   * 
   * @return 
   */
  TA<L,T> & operator += ( L &location){
    locations.push_back( location);
    return *this;
  }

  /** 
   * Add one transition to this timed automata
   * 
   * @param edge 
   * 
   * @return 
   */
  TA<L,T> & operator += ( T &edge){
    transitions.push_back( edge);
    return *this;
  }
  
};

} // namespace ftma

#endif
