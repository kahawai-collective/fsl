namespace Fisl {
namespace Management {
namespace Procedures {

template<class Derived>
class Evaluation {
public:

    int run(void){
        static_cast<Derived*>(this)->conditioner.run();
        return 0;
    }
};

}
}
}
