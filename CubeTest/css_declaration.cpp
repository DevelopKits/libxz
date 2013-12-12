#include "stdafx.h"
#include "css_declaration.h"
#include "css_property.h"

namespace css
{


	Declaration::Declaration()
	{

	}

	Declaration::~Declaration()
	{
		for (auto iter : mData)
		{
			delete iter.second;
		}
	}

	void Declaration::RemoveProperty( PropertyId aProperty )
	{
		if (Property::IsShorthand(aProperty)) {
			//CSSPROPS_FOR_SHORTHAND_SUBPROPERTIES(p, aProperty) {
			//	data.ClearLonghandProperty(*p);
			//	mOrder.RemoveElement(*p);
			//}
		} else {
			//data.ClearLonghandProperty(aProperty);
			//mOrder.RemoveElement(aProperty);
			mData.erase(aProperty);
		}
	}

	bool Declaration::HasProperty( PropertyId aProperty ) const
	{
		return mData.count(aProperty) > 0;
	}

	void Declaration::GetValue( PropertyId aProperty, std::string& aValue ) const
	{
		if (!Property::IsShorthand(aProperty)) {
			AppendValueToString(aProperty, aValue);
			return;
		}
	}

	int32 Declaration::Count() const
	{
		return mData.size();
	}

	void Declaration::ToString( std::string& aString ) const
	{

	}

	void Declaration::InitializeEmpty()
	{

	}

	void Declaration::ClearData()
	{

	}

	bool Declaration::AppendValueToString( PropertyId aProperty, std::string& aResult ) const
	{
		if (!mData.count(aProperty))
			return false;

		Value* val = mData.at(aProperty);

		val->AppendToString(aProperty, aResult);
		return true;
	}

	/*void Declaration::AppendPropertyAndValueToString( PropertyId aProperty, std::string& aValue, std::string& aResult ) const
	{

	}*/

}