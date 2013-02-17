#pragma once
#include <memory>
#include <map>
#include <tuple>
#include <string>
#include <vector>
#include <list>
#include <opencv2/opencv.hpp>

namespace CVUtil
{
	namespace ReadWrite
	{
#pragma region template / interface
		template<typename ArrayIn, typename ArrayOut>
		ArrayOut convertArray(const ArrayIn& src)
		{
			ArrayOut out;
			for (ArrayIn::const_iterator it=src.begin(); it!=src.end(); it++)
				out.push_back(*it);
			return out;
		}

		// Template to use cv::FileStream
		// Example: fs << "Value" << (ICVSaveLoad&)v;
		class ICVSaveLoad
		{
		public:
			virtual void save(cv::FileStorage& fs) const = 0;
			virtual void load(cv::FileNodeIterator& it) = 0;
			virtual void load(const cv::FileNode& node) = 0;
			//template<typename T> virtual operator ICVSaveLoad::Capsule<T>() = 0;
			//template<typename T> virtual operator const ICVSaveLoad::Capsule<T>() const = 0;
		public:
			// Pointer carryer of ICVSaveLoad
			template<typename T> class Capsule
			{
			private:
				static void noDeleting(ICVSaveLoad*) {}
			public:
				std::shared_ptr<ICVSaveLoad> ptr;
				Capsule() : ptr(new T) {}
				Capsule(ICVSaveLoad* c) : ptr(c, noDeleting) {}
				Capsule(T* c) : ptr((ICVSaveLoad*)c, noDeleting) {}
				//Capsule(const T& c) : ptr(&(const ICVSaveLoad&)c, noDeleting) {}
				Capsule(const Capsule& src) : ptr(src.ptr) {}
				Capsule& operator=(const Capsule& src) { ptr = src.ptr; }
				operator T&() { return *ptr; }
				operator const T&() const { return *dynamic_cast<T*>(ptr.get()); }
			};
		};

		template<typename T> cv::FileStorage& operator<<(cv::FileStorage& fs, const ICVSaveLoad::Capsule<T>& c)
		{
			fs << *c.ptr;
			return fs;
		}
		template<typename T> cv::FileNodeIterator& operator>>(cv::FileNodeIterator& it, ICVSaveLoad::Capsule<T>& c)
		{
			it >> *c.ptr;
			return it;
		}
		template<typename T> void operator>>(const cv::FileNode& fn, ICVSaveLoad::Capsule<T>& c)
		{
			fn >> *c.ptr;
		}

		cv::FileStorage& operator<<(cv::FileStorage& fs, const ICVSaveLoad& c);
		cv::FileNodeIterator& operator>>(cv::FileNodeIterator& it, ICVSaveLoad& c);
		void operator>>(const cv::FileNode& fn, ICVSaveLoad& c);

#pragma endregion

#pragma region for standard template library
#pragma region std::map 
		template<typename Tk, typename Tv>
		cv::FileStorage& operator<<(cv::FileStorage& fs, const std::map<Tk, Tv>& map)
		{
			if (!fs.isOpened()) return fs;

			fs << "[";
			for (const auto& item : map)
			{
				fs << "{:";
				fs << "key" << item.first;
				fs << "value" << item.second;
				fs << "}";
			}
			fs << "]";
			return fs;
		}
		template<typename Tk, typename Tv>
		cv::FileNodeIterator& operator>>(cv::FileNodeIterator& it, std::map<Tk, Tv>& map)
		{
			Tk key;
			Tv value;
			(*it)["key"] >> key;
			(*it)["value"] >> value;
			map[key] = value;
			return it;
		}
		template<typename Tk, typename Tv>
		void operator>>(const cv::FileNode& fn, std::map<Tk, Tv>& map)
		{
			map.clear();
			for (auto it = fn.begin(); it != fn.end(); it++)
				it >> map;
		}
#pragma endregion
#pragma region std::vector
		//FIXME vector is conflicted cv.
		template<typename T, typename A>
		class vectorWrapper : public ICVSaveLoad
		{
		protected:
			bool kill;
			std::vector<T, A>* ptr;
		private:
			vectorWrapper& operator=(const vectorWrapper& src);
		public:
			vectorWrapper(const std::vector<T, A>& v) : ptr(&v), kill(false) {}
			vectorWrapper() : ptr(new std::vector<T, A>()), kill(true) {}
			~vectorWrapper() { if (kill) delete ptr; }
		public:
			virtual void save(cv::FileStorage& fs) const
			{
				if (!fs.isOpened()) return;

				fs << "[";
				for (const T& v : *ptr) fs << v;
				fs << "]";
			}
			virtual void load(cv::FileNodeIterator& it)
			{
				T v;
				(*it)[0] >> v;
				ptr->push_back(v);
			}
			virtual void load(const cv::FileNode& node)
			{
				ptr->clear();
				for (cv::FileNodeIterator it = node.begin(); it!= node.end(); it++)
					load(it);
			}
		};
		template<typename T, typename A>
		vectorWrapper<T, A> makeVectorWrapper(const std::vector<T, A>& vec) { return vectorWrapper<T, A>(vec); }
#pragma endregion
#pragma region std::list
		template<typename T, typename A>
		cv::FileStorage& operator<<(cv::FileStorage& fs, const std::list<T, A>& c)
		{
			if (!fs.isOpened()) return fs;

			fs << "[";
			for (const auto& v : c) fs << v;
			fs << "]";
			return fs;
		}
		template<typename T, typename A>
		cv::FileNodeIterator& operator>>(cv::FileNodeIterator& it, std::list<T, A>& c)
		{
			T v;
			(*it)[0] >> v;
			c.push_back(v);
			return it;
		}
		template<typename T, typename A>
		void operator>>(const cv::FileNode& fn, std::list<T, A>& c)
		{
			c.clear();
			for (cv::FileNodeIterator it = fn.begin(); it != fn.end(); it++)
			{
				it >> c;
			}
		}
#pragma endregion
#pragma region std::tuple
		//TODO
#pragma endregion
#pragma region std::pair
		template<typename T1, typename T2>
		cv::FileStorage& operator<<(cv::FileStorage& fs, const std::pair<T1, T2>& pair)
		{
			if (!fs.isOpened()) return fs;

			fs << "[";
			fs << "first" << pair.first;
			fs << "second" << pair.second;
			fs << "]";
			return fs;
		}
		template<typename T1, typename T2>
		cv::FileNodeIterator& operator>>(cv::FileNodeIterator& it, std::pair<T1, T2>& pair)
		{
			(*it)["first"] >> pair.first;
			(*it)["second"] >> pair.second;
			return it;
		}
		template<typename T1, typename T2>
		void operator>>(const cv::FileNode& fn, std::pair<T1, T2>& pair)
		{
			fn.begin() >> pair;
		}
#pragma endregion
#pragma endregion 
	}
}