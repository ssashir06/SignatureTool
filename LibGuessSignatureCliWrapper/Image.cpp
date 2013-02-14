#include "Stdafx.h"
#include "Image.h"

namespace Signature { namespace Image { namespace CLI {
	using namespace System;
	using namespace System::Collections;
	using namespace System::Collections::Generic;

	String^ Base::ImageFileName::get()
	{
		return file_name;
	}

	Void Base::ImageFileName::set(String^ value)
	{
		file_name = value;
	}

	String^ Conclusive::Name::get()
	{
		return name;
	}

	Void Conclusive::Name::set(String^ value)
	{
		name = value;
	}

	array<String^>^ Candidate::Assessments::get()
	{
		return assessments;
	}

	Void Candidate::Assessments::set(array<String^>^ value)
	{
		assessments = value;
	}
}}}