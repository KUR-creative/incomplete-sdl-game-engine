#include <utility>
#include "ResourceLibrary.hpp"
using std::make_pair;
using std::pair;
							
bool ResourceLibrary::insert(const string& key, shared_ptr<LoadedData> item){
	auto result = resourceMap.insert( {key, item} );
	return result.second;
}