#include "NDFA_state.h"

bool NDFA_state::operator== (const NDFA_state& nd){

 if(nd.id == this->id){
        return true;
 }
 return false;
}
