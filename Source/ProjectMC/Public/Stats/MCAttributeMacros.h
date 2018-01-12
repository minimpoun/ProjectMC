#pragma once

#define DECLARE_ATTRIBUTE_FUNCTION(PropertyName) static FGameplayAttribute PropertyName##Attribute();													

#define DEFINE_ATTRIBUTE_FUNCTION(PropertyName, ClassName) 																					\
FGameplayAttribute ClassName##::PropertyName##Attribute()																					\
{																																			\
	static UProperty* Property = FindFieldChecked<UProperty>(ClassName##::StaticClass(), GET_MEMBER_NAME_CHECKED(ClassName, PropertyName));	\
	return FGameplayAttribute(Property);																									\
}																																				

#define DECLARE_NAMED_COMBINED_STAT_GETTER(BaseProperty, BonusProperty, FunctionName)__forceinline float FunctionName##() const				\
{																																			\
	return BaseProperty##.GetCurrentValue() + BonusProperty##.GetCurrentValue();															\
}