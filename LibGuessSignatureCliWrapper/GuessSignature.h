#pragma once
#include <list>
#include <string>
#include <opencv2/opencv.hpp>
#include "IGuessSignature.h"
#include "../cv_utils/cv_utils_cli.h"
#include "../LibGuessSignature/GuessSignature.h"
#include "../LibGuessSignature/GuessEvalEasy.h"

namespace Signature { namespace Guess { namespace CLI {
	using namespace System;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// T ... an inheritance class of Signature::Guess::Base
	/// </summary>
	template<class T> public ref class Base abstract : public IGuessSignature
	{
	protected:
		Guess::Base* guess;
	public:
		Base()
		{
			guess = new T();
		}

		~Base()
		{
			delete guess;
		}

		virtual Void Train(Conclusives^ trainers, Boolean adding)
		{
			std::list<Image::Conclusive> trains;
			for each (Conclusive^ conclusive in trainers)
			{
				trains.push_back(Image::Conclusive(
					CVUtil::CLI::convertString(conclusive->Name),
					CVUtil::CLI::convertString(conclusive->FileName),
					conclusive->Trimming.HasValue?
						std::make_shared<cv::Rect>(conclusive->Trimming.Value.X, conclusive->Trimming.Value.Y, conclusive->Trimming.Value.Width, conclusive->Trimming.Value.Height):
						std::shared_ptr<cv::Rect>(nullptr)));
			}

			guess->train(trains, adding);
		}

		virtual Assessments^ Match(SamplingImage^ image)
		{
			std::shared_ptr<cv::Rect> cv_rect = nullptr;
			if (image->Trimming.HasValue)
				cv_rect = std::make_shared<cv::Rect>(image->Trimming.Value.X, image->Trimming.Value.Y, image->Trimming.Value.Width, image->Trimming.Value.Height);

			Image::Candidate query(CVUtil::CLI::convertString(image->FileName), cv_rect);
			Image::Candidate::Assessments assessments;
			try {
				assessments = guess->match((const Image::Candidate)query);
			} catch (const std::exception& e)
			{
				//FIXME
				throw gcnew Exception("Matching error has occourd: " + CVUtil::CLI::convertString(e.what()));
			} catch (SEHException^ e)
			{
				//FIXME
				throw e;
			}
			return GetCliAssessments(assessments, image);
		}

		virtual Void SaveModel(String^ filename)
		{
			guess->saveModel(CVUtil::CLI::convertString(filename));
		}

		virtual Void LoadModel(String^ filename)
		{
			if (!guess) throw gcnew Exception("Model pointer is currently nullptr.");
			guess->loadModel(CVUtil::CLI::convertString(filename));
		}

		virtual property String^ ModelFileSuffixFilter
		{
			String^ get()
			{
				if (!guess) return nullptr;
				auto suffix = guess->modelSuffix();
				return String::Format("{0} ({1})|*{1}",
					CVUtil::CLI::convertString(suffix.second),
					CVUtil::CLI::convertString(suffix.first));
			}
		}

		virtual property String^ ModelFileSuffix
		{
			String^ get()
			{
				if (!guess) return nullptr;
				auto suffix = guess->modelSuffix();
				return CVUtil::CLI::convertString(suffix.first);
			}
		}

		virtual Void Strip() = 0;
	protected:
		Assessments^ GetCliAssessments(const Image::Candidate::Assessments& assessments, SamplingImage^ image)
		{
			Assessments^ result = gcnew Assessments;
			for (const auto& assessment : assessments)
			{
				Assessment^ assessment_cli = gcnew Assessment();
				assessment_cli->Score = assessment.score;
				assessment_cli->Name = CVUtil::CLI::convertString(assessment.name);
				assessment_cli->FileName = image->FileName;
				assessment_cli->Trimming = image->Trimming;
				result->Add(assessment_cli);
			}
			return result;
		}
	};

	//typedef Base<Guess::EvalEasy> GuessSignatureEvalEasy;
	public ref class GuessSignatureEvalEasy : public Base<Guess::EvalEasy>
	{
	public:
		GuessSignatureEvalEasy() : Base() {}
		~GuessSignatureEvalEasy() {}

		virtual Void Strip() override
		{
			if (!guess) return;
			guess->strip(false);
		}
	};
}}}