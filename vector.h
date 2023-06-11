template<typename vector>
class vector_iterator {
	public:
	using value_type = vector::value_type;
	using pointer = value_type*;
	using reference = value_type&;

	vector_iterator(pointer ptr) : m_ptr(ptr) {}

	vector_iterator& operator++() {
		++m_ptr;
		return *this;
	}

	vector_iterator operator++(int) {
		auto temp = *this;
		++(*this);
		return temp;
	}

	vector_iterator& operator--() {
		--m_ptr;
		return *this;
	}

	vector_iterator operator--(int) {
		auto temp = *this;
		--(*this);
		return temp;
	}
	
	reference operator*() {
		return *m_ptr;
	}

	pointer operator->() {
		return m_ptr;
	}

	reference operator[](int index) {
		return *(m_ptr + index);
	}

	bool operator==(const vector_iterator other) const {
		return this->m_ptr == other.m_ptr;
	}

	bool operator!=(const vector_iterator other) const {
		return !(*this == other);
	}

	private:
	pointer m_ptr;
};

template<typename T>
class vector {
	private:
	T* _arr;
	size_t _size;
	int _idx;

	public:
	using value_type = T;
	using iterator = vector_iterator<vector<T>>;

	vector(size_t size = 8) {
		_arr = new T[size];
		this->_size = size;
		this->_idx = 0;
	}

	~vector() {
		delete[] _arr;
	}

	vector(vector&& v) noexcept {
		printf("move constructor\n");
		this->_arr = v._arr;
		this->_size = v._size;
		this->_idx = v._idx;
		v._arr = nullptr;
	}

	vector(const vector& v) {
		printf("copy constructor\n");
		this->_size = v._size;
		this->_idx = v._idx;
		this->_arr = new T[this->_size];
		for (int i = 0; i < this->_idx; ++i) {
			this->_arr[i] = v._arr[i];
		}
	}

	vector& operator=(const vector& v) {
		printf("copy assignment\n");

		if (this == &v) return *this;
		delete[] this->_arr;
		this->_size = v._size;
		this->_idx = v._idx;
		this->_arr = new T[this->_size];
		for (int i = 0; i < this->_idx; ++i) {
			this->_arr[i] = v._arr[i];
		}
		return *this;
	}

	vector& operator=(vector&& v) {
		printf("move assignment\n");
		if (this == &v) return *this;

		this->_arr = v._arr;
		this->_idx = v._idx;
		this->_size = v._size;
		v._arr = nullptr;
		return *this;
	}

	T& operator[](int i) {
		return this->_arr[i];
	}

	void push_back(const T& value) {
		this->emplace_back(std::move(value));
	}
	void push_back(T&& value) {
		this->emplace_back(std::move(value));
	}

	int size() {
		return this->_idx;
	}

	template<class... Args>
	void emplace_back(Args&&... args) {
		if (this->_idx + 1 == this->_size) {
			T* newarr = new T[this->_size * 2];
			for (auto i = 0; i < _size; ++i) {
				newarr[i] = std::move(this->_arr[i]);
			}
			auto temp = _arr;
			this->_arr = newarr;
			delete[] temp;
			this->_size *= 2;
		}

		this->_arr[this->_idx] = T(std::forward<Args>(args)...);
		this->_idx++;
	}

	iterator begin() {
		return iterator(this->_arr);
	}

	iterator end() {
		return iterator(this->_arr + this->_idx);
	}
};