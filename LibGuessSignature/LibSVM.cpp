#include <cstring>
#include "LibSVM.h"
using namespace std;

namespace LibSVM
{
#pragma region Node
	Node EndOfNode()
	{
		Node n;
		n.index = -1;
		return n;
	}
#pragma endregion

#pragma region Problem
	Problem::Problem()
		: svm_problem(), delete_function(NULL)
	{
		update();
	}

	Problem::Problem(const vector<double>& y, const vector<vector<Node> >& x)
		: svm_problem(), class_numbers(y), nodes(x), delete_function(NULL)
	{
		update();
	}

	Problem::Problem(const Problem& src)
		: svm_problem(), class_numbers(src.class_numbers), nodes(src.nodes), delete_function(src.delete_function)
	{
		l = src.l;
		y = src.y;
		x = src.x;
		update();
	}

	Problem& Problem::operator=(const Problem& src)
	{
	}

	Problem::~Problem()
	{
		if (delete_function) (*delete_function)(this);
	}

	void Problem::setNodes(const vector<double>& y, const vector<vector<Node> >& x)
	{
		class_numbers = y;
		nodes = x;
		update();
	}

	const vector<double>& Problem::getY() const
	{
		return class_numbers;
	}

	const vector<vector<Node> >& Problem::getX() const
	{
		return nodes;
	}

	void Problem::setDeleteFunction(void (*f)(const Problem* target))
	{
		delete_function = f;
	}

	void Problem::update()
	{
		if (class_numbers.size() != nodes.size()) throw "size of arrays is wrong.";
		if (class_numbers.size() == 0) return;

		l = class_numbers.size();
		list_of_nodes.clear();
		for (auto it=nodes.begin(); it!=nodes.end(); it++)
		{
			if (it->empty()) continue;
			if (it->back().index != -1) it->push_back(EndOfNode());
			list_of_nodes.push_back(it->data());
		}
		x = list_of_nodes.data();
		y = class_numbers.data();
	}
#pragma endregion

#pragma region Parameter
	Parameter::Parameter()
		: svm_parameter(), will_destroy(true)
	{
		weight = NULL, weight_label = NULL;
	}

	Parameter::Parameter(const Parameter& src)
		: svm_parameter(src), will_destroy(false)
	{
		CopyArray(weight, src.weight);
		CopyArray(weight_label, src.weight_label);
	}

	Parameter& Parameter::operator=(const Parameter& src)
	{
	}

	Parameter::Parameter(const svm_parameter& param)
		: svm_parameter(param), will_destroy(false)
	{
	}

	Parameter::~Parameter()
	{
		svm_destroy_param(this);
	}
#pragma endregion

#pragma region Model
	Model::Model()
		: svm_model(), will_destroy(true)
	{
		SV = NULL;
		sv_coef = NULL;
		rho = NULL;
		probA = NULL;
		probB = NULL;
		sv_indices = NULL;
		label = NULL;
		nSV = NULL;
	}

	Model::Model(const Model& src)
		: svm_model(src), will_destroy(true)
	{
		// see svm_free_model_content

		CopyArray(SV, src.SV);
		CopyArray(sv_coef, src.sv_coef);

		if (src.free_sv && src.l>0 && src.SV!=NULL)
			CopyArray(SV[0], src.SV[1]);
		if (src.sv_coef)
			for (int i=0; i<src.nr_class-1; i++)
				CopyArray(sv_coef[i], src.sv_coef[i]);
		CopyArray(rho, src.rho);
		CopyArray(probA, src.probA);
		CopyArray(probB, src.probB);
		CopyArray(sv_indices, src.sv_indices);
		CopyArray(label, src.label);
		CopyArray(nSV, src.nSV);
	}

	Model& Model::operator=(const Model& src)
	{
	}

	Model::Model(const svm_model& model)
		: svm_model(model), will_destroy(false)
	{
	}

	Model::~Model()
	{
		if (will_destroy)
			svm_free_model_content(this);
	}
#pragma endregion
}
