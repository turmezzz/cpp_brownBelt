namespace RAII {

    template <class T>
    class Booking {
        T* provider_d = nullptr;
        int id_d;

    public:
        Booking(T* provider_i, int id_i) : provider_d(provider_i),
                                                    id_d(id_i) {
        }

        ~Booking() {
            if (not provider_d) {
                return;
            }
            provider_d->CancelOrComplete(*this);
        }

    };

}