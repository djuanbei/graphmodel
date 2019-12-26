#include <map>

#include "alg/symmetry.h"

namespace graphsat {

bool Symmetry::isSymmetry(const StateSet<UINT> &sets,
                          const ReachableSet<TMStateManager> &reachSet,
                          const TMStateManager *manager) const {

  vector<int> dummy(len);
  vector<int>  index( len);
  int *temp = manager->newState();
  UINT *comTemp=new UINT[ reachSet.getCompressionSize( )];
  for (int i = 0; i < len; i++) {
    dummy[i] = i;
  }
  bool re=true;
  for (auto e : sets) {
    sort(dummy.begin(), dummy.end());

    while (next_permutation(dummy.begin(), dummy.end())) {
      for (int i = 0; i < len; i++) {
        index[i] = i;
      }
 
      reachSet.decode(e, temp);
      manager->dump(temp);

      for (int i = 0; i < len; i++) {
        if (dummy[i] != index[i]) {
          int j=i;
          for(;j< len; j++){
            if( dummy[ j]==index[ i]){
              break;
            }
          }
          assert( j<len);
          manager->swap(j , i, temp);
          int tt=index[ i];
          index[ i]=index[ j];
          index[ j]=tt;
        }
      }
      cout<<"==========="<<endl;
       manager->dump(temp);
       cout<<"==========="<<endl;
      reachSet.encode(temp, comTemp );

      if (!sets.contain(comTemp )){
        assert(false);
        re=false;
        break;
      }

    }
    if( !re){
      break;
    }
  }
  manager->destroyState(temp);
  delete[ ] comTemp;
  return re;
}

} // namespace graphsat
