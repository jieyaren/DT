#ifndef INDEX_H
#define INDEX_H

#define INDEX_MAX 255
//计数器 ，循环255
class index
{
protected:
	index() :i(0),free_count(value){}
public:
	enum{  value = INDEX_MAX,  };
	int alloc()
	{
		if (free_count)
		{
			free_count--;
			int i_=i;
			for (; i_ < value; ++i_)
			{
				if (iflag[i_].is_could_use())
				break;
			}
			i = (i_+value+1)%value;
			iflag[i_].set(true);
			return i_;
		}
		else
		{
			int i_ = i;
			i = (i + value + 1) % value;
			return i_;
		}
	}
	void free(int i)
	{
		free_count++;
		iflag[i].set(false);
	}
	static index& instance()
	{
		 static index s;
		 return s;
	}
private:
	class __bool
	{
		bool __b;
	public:
		__bool():__b(false){}
		bool is_could_use() const {return __b !=true; }
		void set(const bool b) { __b =b;}
	};
	__bool iflag[value] ;
	int i;
	int free_count;
};

#endif // INDEX_H
