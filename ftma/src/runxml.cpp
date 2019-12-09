#include "io/runxml.h"
#include "io/uppaalmodelparser.h"
#include "state/reachableset.hpp"

namespace graphsat {

typedef ReachableSet<INT_TAS_t::StateManager_t> R_t;

void runxml( const std::string &file_name ) {
  UppaalParser              parser( file_name );
  INT_TAS_t                 sys     = parser.getSYS();
  shared_ptr<INT_TAS_t::StateManager_t> manager = sys.getStateManager();
  R_t                       data( manager );

  sys.addInitState( data);

  Reachability<INT_TAS_t> reacher( sys );

  reacher.computeAllReachableSet( data );
  cout << "reach data size: " << data.size() << endl;
  data.generatorDot( "test.gv" );
}

} // namespace graphsat
