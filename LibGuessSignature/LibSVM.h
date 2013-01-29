#pragma once
#include <vector>
#include <cstdlib>
#include <svm.h>

namespace LibSVM
{
	typedef svm_node Node;

	/*constexpr */Node EndOfNode();

	template<typename T>
	void CopyArray(T& dst, const T& src)
	{
		if (src == NULL) {
			dst = NULL;
			return;
		}
#ifdef _WIN32
		size_t allocated_size = _msize(src);
#else
		size_t allocated_size = malloc_usable_size(src);
#endif
		allocated_size -= allocated_size % sizeof(T);

		dst = malloc(allocated_size);
		memcpy(dst, src, allocated_size);
	}

	class Problem : public svm_problem
	{
	protected:
		std::vector<Node*> list_of_nodes;
		std::vector<double> class_numbers;
		std::vector<std::vector<Node> > nodes;
		void (*delete_function)(const Problem* target);
	public:
		Problem();
		Problem(const std::vector<double>& y, const std::vector<std::vector<Node> >& x);
		Problem(const Problem& src);
		Problem& operator=(const Problem& src);
		virtual ~Problem();

		void setNodes(const std::vector<double>& y, const std::vector<std::vector<Node> >& x);
		const std::vector<double>& getY() const;
		const std::vector<std::vector<Node> >& getX() const;
		std::pair<double, const std::vector<Node>& > getNode(size_t idx) const;
		void setDeleteFunction(void (*f)(const Problem* target));
	protected:
		void update();
	};

	class Parameter : public svm_parameter
	{
	private:
		const bool will_destroy;
	public:
		Parameter();
		Parameter(const Parameter& src);
		Parameter& operator=(const Parameter& src);
		Parameter(const svm_parameter& param);
		virtual ~Parameter();
	};

	class Model : public svm_model
	{
	private:
		const bool will_destroy;
	public:
		Model();
		Model(const Model& src);
		Model& operator=(const Model& src);
		Model(const svm_model& model);
		virtual  ~Model();
	};
}
