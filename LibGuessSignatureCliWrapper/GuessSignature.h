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

	// T ... an inheritance class of Signature::Guess::Base
	template<class T>
	ref class Base : public IGuessSignature
	{
	protected:
		Conclusives^ training_filenames;
		Guess::Base* guess;
	public:
		Base()
		{
			guess = new T();
			assessments = gcnew Assessments();
		}

		~Base()
		{
			delete guess;
		}

		property Conclusives^ TrainingImageFiles
		{
			Conclusives^ get()
			{
				return training_filenames;
			}
		}

		Void train()
		{
			std::list<Image::Conclusive> trains;
			for each (Conclusive^ conclusive in training_filenames)
			{
				trains.push_back(Image::Conclusive(
					CVUtil::CLI::convertString(conclusive->Name),
					CVUtil::CLI::convertString(conclusive->FileName)));
			}

			guess->train(trains);
		}

		Assessments^ match(String^ filename_of_query_image)
		{
			Image::Candidate query(CVUtil::CLI::convertString(filename_of_query_image));
			Image::Candidate::Assessments assessments = guess->match(query);
			Assessments^ result = gcnew Assessments;
			for (const auto& assessment : assessments)
			{
				Assessment^ assessment_cli = gcnew Assessment();
				assessment_cli->name = CVUtil::CLI::convertString(assessment.name);
				assessment_cli->Score = assessment.score;
				result->Add(assessment_cli);
			}
			return result;
		}
	};

	typedef Base<Guess::EvalEasy> GuessSignatureEvalEasy;
}}}