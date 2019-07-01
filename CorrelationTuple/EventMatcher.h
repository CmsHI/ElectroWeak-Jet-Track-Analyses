/*
 *  EventMatcher.h
 *
 *  Author: Kaya Tatar
 *
 *  class to match LHC collision events at CMS by run, lumi and event numbers
 */

#ifndef EVENTMATCHER_H_
#define EVENTMATCHER_H_

#include <map>

class EventMatcher {
public:
    EventMatcher(){};
    ~EventMatcher(){};

    /*
     * entry : entry number of the corresponding event in the TTree.
     */
    bool addEvent(unsigned int run, unsigned int lumi, unsigned long long event, unsigned long long entry){
        std::pair<Key, long long> pair({run, lumi, event}, entry);
        return map_.insert(pair).second;
    }

    bool removeEvent(unsigned int run, unsigned int lumi, unsigned long long event){
        const Key key = {run, lumi, event};

        // For the key-based version (2), the function returns the number of elements erased, which in map containers is at most 1.
        // Member type size_type is an unsigned integral type.
        int numRemoved = map_.erase(key);
        return (numRemoved == 1) ? true : false;
    }

    long long getEntry(unsigned int run, unsigned int lumi, unsigned long long event) {
        Key key = {run, lumi, event};
        iterator_map iterator = map_.find(key);

        if(iterator == map_.end() ) {
            return -1;
        } else {
            return iterator->second;
        }
    }

    bool containsEvent(unsigned int run, unsigned int lumi, unsigned long long event) {
        return (getEntry(run, lumi, event) == -1) ? false : true;
    }

    long long getSize() {
        return map_.size();
    }

    void clear() {
        map_.clear();
    }

private:

    struct Key {
      unsigned int run, lumi;
      unsigned long long event;

        /*
         * comparison order : run, lumi, event
         * */
        bool operator < (const Key & other) const {
            if (run < other.run) return true;
            if (run > other.run) return false;

            if (lumi < other.lumi) return true;
            if (lumi > other.lumi) return false;

            if (event < other.event) return true;
            if (event > other.event) return false;

            return false;
        }
    };


    std::map<Key, unsigned long long> map_;
    typedef std::map<Key, unsigned long long>::const_iterator iterator_map;
};

#endif /* EVENTMATCHER_H_ */
