#pragma once

namespace Signature { namespace Guess { namespace CLI {
	using namespace System;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	ref class Conclusive
	{
	protected:
		String^ name;
		String^ file_name;
	public:
		Conclusive()
		{
		}
		property String^ Name
		{
			String^ get() { return name; }
			Void set(String^ value) { name = value; }
		}
		property String^ FileName
		{
			String^ get() { return file_name; }
			Void set(String^ value) { file_name = value; }
		}
	};
	ref class Assessment : public Conclusive
	{
	protected:
		Double score;
	public:
		Assessment()
			: Conclusive()
		{
		}
		property Double Score
		{
			Double get() { return score; }
			Void set(Double value) { score = value; }
		}
	};
	typedef List<Conclusive^> Conclusives;
	typedef List<Assessment^> Assessments;
	interface class IGuessSignature
	{
		property Conclusives^ TrainingImageFiles
		{
			Conclusives^ get();
		}

		Void train();
		Assessments^ match(String^ filename_of_query_image);
	};
}}}