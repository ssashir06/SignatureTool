#pragma once

namespace Signature { namespace Guess { namespace CLI {
	using namespace System;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Drawing;
	public ref class SamplingImage
	{
	protected:
		String^ file_name;
		Nullable<Rectangle> trimming;
	public:
		SamplingImage()
		{
		}

		SamplingImage(String^ file_name)
			: file_name(file_name)
		{
		}

		SamplingImage(String^ file_name, Nullable<Rectangle> trimming_area)
			: file_name(file_name), trimming(trimming_area)
		{
		}

		property String^ FileName
		{
			String^ get() { return file_name; }
			Void set(String^ value) { file_name = value; }
		}
		property Nullable<Rectangle> Trimming
		{
			Nullable<Rectangle> get() { return trimming; }
			Void set(Nullable<Rectangle> value) { trimming = value; }
		}
	};
	public ref class Conclusive : public SamplingImage
	{
	protected:
		String^ name;
	public:
		Conclusive()
			: SamplingImage()
		{
		}

		Conclusive(String^ file_name, String^ name)
			: SamplingImage(file_name), name(name)
		{
		}

		Conclusive(String^ file_name, Nullable<Rectangle> trimming_area, String^ name)
			: SamplingImage(file_name, trimming_area), name(name)
		{
		}

		property String^ Name
		{
			String^ get() { return name; }
			Void set(String^ value) { name = value; }
		}
	};
	public ref class Assessment : public Conclusive
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
	public interface class IGuessSignature
	{
		Void Train(Conclusives^ trainers, Boolean adding);
		Assessments^ Match(SamplingImage^ image);

		Void SaveModel(String^ filename);
		Void LoadModel(String^ filename);
		String^ ModelFileSuffixFilter();

		Void Strip();
	};
}}}