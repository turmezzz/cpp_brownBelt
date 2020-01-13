#include "Common_cache.h"

#include <list>
#include <mutex>

using namespace std;

class LruCache : public ICache {
private:
    shared_ptr <IBooksUnpacker> books_unpacker_d = nullptr;
    list <BookPtr> cache_d;
    size_t used_memory_d = 0;
    Settings settings_d;
    mutex m;

    void CleanCache() {
        while (not cache_d.empty() and used_memory_d > settings_d.max_memory) {
            used_memory_d -= cache_d.back()->GetContent().length() * sizeof(char);
            cache_d.pop_back();
        }
    }

public:
    LruCache(shared_ptr <IBooksUnpacker> books_unpacker,
             const Settings& settings) : books_unpacker_d(books_unpacker), settings_d(settings) {
    }

    BookPtr GetBook(const string& book_name) override {
        unique_lock <mutex> lock(m);
        auto book_it = cache_d.end();
        for (auto it = cache_d.begin(); it != cache_d.end(); ++it) {
            if ((*it)->GetName() == book_name) {
                book_it = it;
                break;
            }
        }
        BookPtr ret_book = nullptr;
        if (book_it == cache_d.end()) {
            BookPtr new_book = books_unpacker_d->UnpackBook(book_name);
            used_memory_d += new_book->GetContent().length() * sizeof(char);
            cache_d.push_front(new_book);
            ret_book = new_book;
        } else {
            BookPtr tmp_book = *book_it;
            cache_d.erase(book_it);
            cache_d.push_front(tmp_book);
            ret_book = tmp_book;
        }
        CleanCache();
        return ret_book;
    }
};


unique_ptr <ICache> MakeCache(shared_ptr <IBooksUnpacker> books_unpacker,
                              const ICache::Settings& settings) {

    return make_unique <LruCache>(books_unpacker, settings);
}
