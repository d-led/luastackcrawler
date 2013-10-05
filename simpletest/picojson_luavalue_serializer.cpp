#include "picojson_luavalue_serializer.h"
#include <algorithm>

namespace picojson {
	namespace convert {

		namespace visitors {

			static std::vector<const void*> done; //hack around static_visitor
			bool Done(const void* v)
			{
				return std::find(done.begin(),done.end(),v)!=done.end();
			}
			void AddDone(const void* v)
			{
				done.push_back(v);
			}
			void ClearDone()
			{
				done.clear();
			}
		}

		namespace visitors {
			value _ToValue(LuaMultiValue const& v)
			{
				return boost::apply_visitor( visitors::tovalue_visitor<value>(), v );
			}

			value GetValue(LuaTable const& T)
			{
				::picojson::array a;
				for (LuaTable::EntryContainer::const_iterator it=T.begin(); it!=T.end();++it) {
					value key = _ToValue(it->first);
					value val = _ToValue(it->second);
					::picojson::object o;
					o["Key"]=key;
					o["Value"]=val;
					a.push_back(value(o));
				}
				return value(a);
			}
		}
	}
}

SerializableLuaStack::SerializableLuaStack(LuaStack const& S):
Values(S.begin(),S.end()) { }

::picojson::value SerializableLuaStack::Value() {
	return ::picojson::convert::to_value(Values);
}
