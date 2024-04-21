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
		unsigned int size() const {
			return difference;
		}
		unsigned int maxSize() const {
			return buffer.size();
		}
		T& operator[](unsigned int i) {
			return buffer[i];
		}
		T& front() {
			return buffer[readPosition];
		}
		T& back() {
			return buffer[writePosition];
		}
		void push() {
			moveWrite();
			difference++;
			// if we've pushed one too many elements to the back,
			// we need to drop to drop one from the front
			if(difference > maxSize()) {
				difference--;
				moveRead();
			}
		}
		void pop() {
			// only pop if there are elements to pop
			if(difference > 0) {
				moveRead();
				difference--;
			}
		}
	};
}