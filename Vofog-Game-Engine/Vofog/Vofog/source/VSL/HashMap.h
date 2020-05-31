#pragma once
#include <Vofog.h>
#include <Rendering/Renderables/Material.h>

namespace VofogUtils{


	const int TABLE_SIZE = 1000;
	class HashNode{
	public:
		int key;
		int value;
		HashNode* next;
		HashNode(int key, int value){
			this->key = key;
			this->value = value;
			this->next = NULL;
		}
	};


	class HashMap{
	private:
		HashNode** _hashTable;
		std::vector<int> _keys;
	public:
		HashMap();
		~HashMap();
		/*
		 * Hash Function
		 */
		int hashFunction(int key);

		/*
		 * Insert Element at a key
		 */
		void insert(int key, int value);
		/*
		 * Remove Element at a key
		 */
		void remove(int key);
		/*
		 * Search Element at a key
		 */
		int search(int key);

		/*
		 * Get Int* representing top of list
		 */

		HashNode* getHeadOfChain(int key);


		std::vector<int> getKeys(){ return _keys; }

	};

}