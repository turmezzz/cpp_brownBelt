#include <memory>

namespace RAII {
    template <class ProviderT>
    class Booking {
    private:
        ProviderT* provider_;
        int id_;

    public:
        Booking(ProviderT* provider, int id) : provider_(provider),
                                               id_(id) {
        }

        Booking() = delete;

        Booking(Booking&& other) : provider_(other.provider_), id_(other.id_) {
            ++ProviderT::counter;
        }

        Booking(const Booking&) = delete;

        Booking& operator=(Booking&& other) {
            if (other.provider_ == provider_ and other.id_ == id_) {
                return *this;
            }

            provider_ = other.provider_;
            id_ = other.id_;

            other.provider_ = nullptr;
            other.id_ = 0;


            ++ProviderT::counter;
            return *this;
        }

        Booking& operator=(const Booking&) = delete;

        ~Booking() {
            provider_->CancelOrComplete(*this);
        }
    };
}