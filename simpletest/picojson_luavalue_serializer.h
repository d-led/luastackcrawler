#pragma once

#include <picojson_vector_serializer.h>
#include "../luatablestack/luavalue.h"
#include "../luatablestack/luastack.h"
#include <sstream>
#include <iomanip>

namespace picojson {
	namespace convert {

		namespace visitors {

			template <typename TPtr>
			std::string to_addr(std::string const& title,TPtr p) {
				//table: 0x7fcd2243ef90
				std::stringstream ss;
				ss<<title<<": "<<std::noshowbase<<std::hex<<p;
				return ss.str();
			} 

			value _ToValue(LuaMultiValue const& v);
			value GetValue(LuaTable const& T);
			bool Done(const void* v);
			void AddDone(const void* v);
			void ClearDone();

			template <typename TValue>
			class tovalue_visitor : public boost::static_visitor<TValue>
			{
			public:
				TValue operator()(bool const& v) const
				{
					return TValue(v);
				}

				TValue operator()(double const& v) const
				{
					return TValue(v);
				}

				TValue operator()(std::string const& v) const
				{
					return TValue(v);
				}

				TValue operator()(boost::shared_ptr<LuaTable> const& v) const
				{
					if (!v) return TValue();
					bool already_done=Done(v.get());
					if (!already_done) {
						AddDone(v.get());
						return GetValue(*v);
					}
					::picojson::array a;
					a.push_back( value( to_addr( "table",v->lua_ptr() ) ) );
					return value(a);
				}

				TValue operator()(LuaFunction const& v) const
				{
					return TValue(std::string("function"));
				}

				TValue operator()(LuaThread const& v) const
				{
					return TValue(std::string("thread"));
				}

				TValue operator()(LuaUserdata const& v) const
				{
					return TValue(std::string("userdata"));
				}

				TValue operator()(LuaNil const& v) const
				{
					return TValue();
				}
			};
		}

		template<> struct value_converter< LuaMultiValue > {

			static value to_value(LuaMultiValue const& v) {
				return boost::apply_visitor( visitors::tovalue_visitor<value>(), v );
			}

			static void from_value(value const& ov, LuaMultiValue& v) {
			}
		};

	}
}

class SerializableLuaStack {
public:
	SerializableLuaStack(LuaStack const& S);
	::picojson::value Value();
private:
	std::vector<LuaMultiValue> Values;
public:
	friend class picojson::convert::access;
	template<class Archive>
	void json(Archive & ar)
	{
		ar & ::picojson::convert::member("Values", Values);
	}
};

