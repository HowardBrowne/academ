#pragma once

namespace ofxEdsdk {
	template <class T>
	class FixedQueue {
	protected:
		vector<T> buffer;
		unsigned int readPosition, writePosition, difference;
		void moveRead() {
			readPosition = (readPosition + 1) % maxSize();
		}
		void moveWrite() {
			writePosition = (writePosition + 1) % maxSize();
		}
	public:
		FixedQueue() :
		readPosition(0),
		writePosition(0),
		difference(0) {
		}
		void resize(int size) {
			buffer.resize(size);
		}
		T& operator[](unsigned int i) {
			return buffer[i % maxSize()];
		}
		void push_back(T cur) {
			(*this)[writePosition] = cur;
			moveWrite();
			difference++;
			if(difference > maxSize()) {
				difference--;
				moveRead();
			}
		}
		T pop_front() {
			T cur = (*this)[readPosition];
			if(difference > 0) {
				moveRead();
				difference--;
			}
			return cur;
		}
		unsigned int size() const {
			return difference;
		}
		unsigned int maxSize() const {
			return buffer.size();
		}
	};
}