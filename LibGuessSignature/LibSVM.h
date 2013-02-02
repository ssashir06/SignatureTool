#pragma once
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <tuple>
#include <cstdlib>
#include <svm.h>

namespace LibSVM
{
	typedef svm_node Node;
	static /*constexpr */Node endOfNode();
	/* might be ok
	class Node : public svm_node
	{
	public:
		Node endOfNode();
	};
	*/

	class NodeArray
	{
	protected:
		std::shared_ptr<std::vector<Node> > nodes;
	public:
		typedef std::pair<double, NodeArray> Classified;
	public:
		NodeArray();
		NodeArray(const std::vector<Node>& nodes);
		NodeArray(const std::shared_ptr<std::vector<Node> >& nodes);
		NodeArray(const NodeArray& src);
		NodeArray& operator=(const NodeArray& src);
		virtual ~NodeArray();
		std::shared_ptr<NodeArray> clone() const;

		Node* getPtr() const;
		const std::vector<Node>& getArray() const;
		std::vector<Node>& getArray();
	protected:
		void setupTail();
	};

	class Problem : public svm_problem
	{
	protected:
		std::shared_ptr<std::vector<Node*> > list_of_nodes;
		std::shared_ptr<std::vector<double> > class_numbers;
		std::shared_ptr<std::vector<NodeArray> > node_lists;
		void (*delete_function)(const Problem* target);
	public:
		Problem();
		Problem(const std::vector<NodeArray::Classified>& data);
		Problem(const Problem& src);
		Problem& operator=(const Problem& src);
		virtual ~Problem();

		void setNodes(const std::vector<NodeArray::Classified>& x);
		const std::vector<double>& getY() const;
		const std::vector<NodeArray>& getX() const;
		const NodeArray::Classified& getNode(size_t idx) const;
		void setDeleteFunction(void (*f)(const Problem* target));
	};

	namespace Wrapper
	{
		template<typename T, void (*F)(T*)>
		struct Killer
		{
			void operator()(T* ptr) const
			{
				F(ptr);
			}
		};
		struct NoKiller
		{
			template<typename T>
			void operator()(T* ptr) const
			{
			}
		};
		template<typename T, class D>
		class Wrapper
		{
		protected:
			std::shared_ptr<T> ptr;
		public:
		public:
			Wrapper()
				: ptr(new T(), D())
			{
			}

			Wrapper(T* ptr)
				: ptr(ptr, D())
			{
			}

			Wrapper(const Wrapper& src)
				: ptr(src.ptr)
			{
			}

			Wrapper& operator=(const Wrapper& src)
			{
				ptr = src.ptr;
				return *this;
			}

			virtual ~Wrapper()
			{
			}

			const T* operator->() const
			{
				return ptr.get();
			}

			T* operator->()
			{
				return ptr.get();
			}

			operator T*() const
			{
				return ptr.get();
			}
		};
	}

	typedef protected Wrapper::Wrapper<svm_parameter, Wrapper::Killer<svm_parameter, svm_destroy_param> > Parameter;
	//typedef protected DestroyWrapper<svm_model, svm_free_model_content> Model;
	class Model : public Wrapper::Wrapper<svm_model, Wrapper::NoKiller>
	{
	public:
		Model();
		Model(svm_model* ptr);
		Model(const Model& src);
		Model& operator=(const Model& src);
		virtual ~Model();
	};

	typedef double v_min;
	typedef double v_max;
	typedef std::pair<v_max, v_min> v_range;
	typedef std::tuple<v_min, v_min, double> v_scale;
	typedef int index;
	typedef std::tuple<bool, v_scale, std::map<index, v_scale> > ScalingSetting;

	std::vector<LibSVM::NodeArray::Classified> mergeClassified(const std::list<std::vector<LibSVM::NodeArray::Classified> >& data);
	ScalingSetting buildSettingOfScaling(const std::vector<LibSVM::NodeArray::Classified>& data, const v_range& x_range_new = v_range(1, -1), const v_range& y_range_new = v_range(1, -1), bool scale_y = false);
	void scale(const ScalingSetting& setting, LibSVM::NodeArray& node_array);
	void scale(const ScalingSetting& setting, std::vector<LibSVM::NodeArray::Classified>& classified_data);
}
