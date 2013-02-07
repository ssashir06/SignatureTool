#include <cstring>
#include "LibSVM.h"
using namespace std;

namespace LibSVM
{
#pragma region Node
	Node endOfNode()
	{
		Node n;
		n.index = -1;
		return n;
	}
#pragma endregion

#pragma region NodeArray
	NodeArray::NodeArray()
		: nodes(nullptr)
	{
		setupTail();
	}

	NodeArray::NodeArray(const vector<Node>& nodes)
		: nodes(make_shared<vector<Node> >(nodes))
	{
		setupTail();
	}

	NodeArray::NodeArray(const shared_ptr<vector<Node> >& nodes)
		: nodes(nodes)
	{
		setupTail();
	}

	NodeArray::NodeArray(const NodeArray& src)
		: nodes(src.nodes)
	{
	}

	NodeArray& NodeArray::operator=(const NodeArray& src)
	{
		nodes = shared_ptr<vector<Node> >(src.nodes);
		return *this;
	}

	NodeArray::~NodeArray()
	{
	}

	shared_ptr<NodeArray> NodeArray::clone() const
	{
		auto c = make_shared<NodeArray>();
		c->nodes = make_shared<vector<Node> >(nodes->begin(), nodes->end());
		return c;
	}

	Node* NodeArray::getPtr() const
	{
		if (!nodes) return nullptr;
		else return nodes->data();
	}

	const vector<Node>& NodeArray::getArray() const
	{
		return *nodes;
	}

	vector<Node>& NodeArray::getArray()
	{
		return *nodes;
	}

	void NodeArray::setupTail()
	{
		if (!nodes) nodes = shared_ptr<vector<Node> >(new vector<Node>());
		if (nodes->empty() || nodes->back().index != -1) nodes->push_back(endOfNode());
	}
#pragma endregion

#pragma region Problem
	Problem::Problem()
		: svm_problem(), delete_function(nullptr)
	{
		setNodes(vector<NodeArray::Classified>());
	}

	Problem::Problem(const vector<NodeArray::Classified>& data)
		: svm_problem(), delete_function(nullptr)
	{
		setNodes(data);
	}

	Problem::Problem(const Problem& src)
		: svm_problem(), class_numbers(src.class_numbers), list_of_nodes(src.list_of_nodes), node_lists(src.node_lists), delete_function(src.delete_function)
	{
		if (node_lists && class_numbers) {
			l = list_of_nodes->size();
			x = list_of_nodes->data();
			y = class_numbers->data();
		} else {
			l = 0;
			x = nullptr;
			y = nullptr;
		}
	}

	Problem& Problem::operator=(const Problem& src)
	{
		delete_function = src.delete_function;
		l = src.l;
		list_of_nodes = src.list_of_nodes;
		class_numbers = src.class_numbers;
		node_lists = src.node_lists;
		return *this;
	}

	Problem::~Problem()
	{
		if (delete_function) (*delete_function)(this);
	}

	void Problem::setNodes(const vector<NodeArray::Classified>& data)
	{
		class_numbers = make_shared<vector<double> >();
		node_lists = make_shared<vector<NodeArray> >();
		list_of_nodes = make_shared<vector<Node* > >();
		for (const auto& d : data)
		{
			class_numbers->push_back(d.first);
			node_lists->push_back(d.second);
			list_of_nodes->push_back(node_lists->back().getPtr());
		}

		l = data.size();
		x = list_of_nodes->data();
		y = class_numbers->data();
	}

	const vector<double>& Problem::getY() const
	{
		return *class_numbers;
	}

	const vector<NodeArray>& Problem::getX() const
	{
		return *node_lists;
	}

	void Problem::setDeleteFunction(void (*f)(const Problem* target))
	{
		delete_function = f;
	}
#pragma endregion

#pragma region Model
	Model::Model()
		: Wrapper()
	{
	}

	Model::Model(svm_model* ptr)
		: Wrapper(ptr)
	{
	}

	Model::Model(const Model& src)
		: Wrapper(src)
	{
	}

	Model& Model::operator=(const Model& src)
	{
		Wrapper::operator=(src);
		return *this;
	}

	Model::~Model()
	{
		if (ptr && ptr.unique()) {
			svm_model* ptr_raw = ptr.get();
			svm_free_and_destroy_model(&ptr_raw);
		}
	}
#pragma endregion

#pragma region static functions
	vector<NodeArray::Classified> mergeClassified(const list<vector<NodeArray::Classified> >& data)
	{
		auto data_merged = list<NodeArray::Classified>();
		for (const auto& d1 : data)
			for (const auto& d2 : d1)
				data_merged.push_back(d2);
		return vector<NodeArray::Classified>(data_merged.begin(), data_merged.end());
	}

	ScalingSetting buildSettingOfScaling(const vector<NodeArray::Classified>& data, const v_range& x_range_new, const v_range& y_range_new, bool scale_y)
	{
		// search max and min
		map<index, v_range> x_range;
		v_range y_range(DBL_MAX, DBL_MIN);
		for (const auto& d : data)
		{
			y_range.first = std::max(y_range.first, d.first);
			y_range.second = std::min(y_range.second, d.first);

			for (const auto& d2 : d.second.getArray())
			{
				if (x_range.find(d2.index) == x_range.end()) {
					x_range[d2.index].first = d2.value, x_range[d2.index].second = d2.value;
				} else {
					x_range[d2.index].first = std::max(x_range[d2.index].first, d2.value);
					x_range[d2.index].second = std::min(x_range[d2.index].second, d2.value);
				}
			}
		}

		const v_scale scale_defualt = make_tuple(0.0, 0.0, 1.0);
		ScalingSetting setting = make_tuple(false, scale_defualt, map<index, v_scale>());
		if (scale_y)
			get<1>(setting) = make_tuple(
			y_range.second,
			y_range_new.second,
			(y_range_new.first - y_range_new.second) / (double)(y_range.first - y_range.second));
		for (const auto& xr : x_range)
		{
			auto& map = get<2>(setting);
			if (xr.second.first == xr.second.second)
				map[xr.first] = scale_defualt;
			else
				map[xr.first] = make_tuple(
				xr.second.second,
				x_range_new.second,
				(x_range_new.first - x_range_new.second) / (double)(xr.second.first - xr.second.second));
		}

		return setting;
	}

	void scale(const ScalingSetting& setting, NodeArray& node_array)
	{
		for (auto& d : node_array.getArray())
		{
			// x
			const v_scale& s = get<2>(setting).at(d.index);
			//const v_scale& s = get<2>(setting)[d.index];// it doesn't work
			d.value = (d.value - get<0>(s)) * get<2>(s) + get<1>(s);
		}
	}

	void scale(const ScalingSetting& setting, std::vector<NodeArray::Classified>& classified_data)
	{
		// apply range
		for (auto& d : classified_data)
		{
			// y
			if (get<0>(setting)) {
				const v_scale& s = get<1>(setting);
				d.first = (d.first - get<0>(s)) * get<2>(s) + get<1>(s);
			}
			scale(setting, d.second);
		}
	}
#pragma endregion
}
