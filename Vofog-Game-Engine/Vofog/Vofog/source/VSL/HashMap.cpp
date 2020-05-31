#include "vpch.h"
#include "HashMap.h"

namespace VofogUtils{


	HashMap::HashMap(){
		_hashTable = new HashNode*[TABLE_SIZE];
		for(int i = 0; i < TABLE_SIZE; i++)
			_hashTable[i] = NULL;


	}



	void HashMap::insert(int key, int value){
		int hash_val = hashFunction(key);
		HashNode* prev = NULL;
		HashNode* entry = _hashTable[hash_val];
		while(entry != NULL){
			prev = entry;
			entry = entry->next;
		}
		if(entry == NULL){
			entry = new HashNode(key, value);
			_keys.push_back(value);
			if(prev == NULL){
				_hashTable[hash_val] = entry;
			} else{
				prev->next = entry;
			}
		} else{
			entry->value = value;
		}
	}

	int HashMap::hashFunction(int key){
		return key % TABLE_SIZE;
	}

	void HashMap::remove(int key){
		int hash_val = hashFunction(key);
		HashNode* entry = _hashTable[hash_val];
		HashNode* prev = NULL;
		if(entry == NULL || entry->key != key){
			VOFOG_CORE_WARN("No element found at key {}", key);
			return;
		}
		while(entry->next != NULL){
			prev = entry;
			entry = entry->next;
		}
		if(prev != NULL){
			prev->next = entry->next;
		}
		delete entry;
		VOFOG_CORE_WARN("Element Deleted", key);

	}


	int HashMap::search(int key){
		bool flag = false;
		int hash_val = hashFunction(key);
		HashNode* entry = _hashTable[hash_val];
		while(entry != NULL){
			if(entry->key == key){
				flag = true;
			}
			entry = entry->next;
		}
		if(!flag)
			return -1;
	}

	HashNode* HashMap::getHeadOfChain(int key){
		int hash_val = hashFunction(key);
		return _hashTable[hash_val];
	}

	HashMap::~HashMap(){
		for(int i = 0; i < TABLE_SIZE; ++i){
			HashNode* entry = _hashTable[i];
			while(entry != NULL){
				HashNode* prev = entry;
				entry = entry->next;
				delete prev;
			}
		}
		delete[] _hashTable;
	}


}