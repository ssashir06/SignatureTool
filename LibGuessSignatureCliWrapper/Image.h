#pragma once

namespace Signature { namespace Image { namespace CLI {
	using namespace System;
	using namespace System::Collections;
	using namespace System::Collections::Generic;

	ref class Base
	{
		String^ file_name;
	public:
		property String^ ImageFileName
		{
			String^ get();
			Void set(String^ value);
		}
	};
	ref class Conclusive : public Base
	{
		String^ name;
	public:
		property String^ Name
		{
			String^ get();
			Void set(String^ value);
		}
	};
	ref class Candidate : public Base
	{
		array<String^>^ assessments;
	public:
		property array<String^>^ Assessments
		{
			array<String^>^ get();
			Void set(array<String^>^ value);
		}
	};
}}}