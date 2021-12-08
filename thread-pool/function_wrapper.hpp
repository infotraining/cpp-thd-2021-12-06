#ifndef FUNCTION_WRAPPER_HPP
#define FUNCTION_WRAPPER_HPP

#include <memory>
#include <functional>
#include <cstddef>

namespace StdExtensions
{
    class Function
    {
        struct IFunction
        {
            virtual void call() = 0;
            virtual ~IFunction() = default;
        };

        std::unique_ptr<IFunction> impl_;

        template <typename Callable>
        struct FunctionWrapper : public IFunction
        {
            Callable f;

            FunctionWrapper(Callable&& f)
                : f(std::move(f))
            {
            }

            void call() override
            {
                f();
            }
        };        

    public:
        Function(std::nullptr_t)
        {
        }

        template <typename Callable>
        Function(Callable&& f)
            : impl_{std::make_unique<FunctionWrapper<Callable>>(std::forward<Callable>(f))}
        {
        }

        void operator()()
        {
            if (!impl_)
                throw std::bad_function_call();
          
            impl_->call();
        }

        Function() = default;
        Function(Function&& source) = default;
        Function& operator=(Function&&) = default;

        Function(const Function& source) = delete;
        Function& operator=(const Function&) = delete;

        bool operator==(std::nullptr_t ptr)
        {
            return impl_.get() == ptr;
        }

        bool empty() const
        {
            return impl_.get() == nullptr;
        }

        explicit operator bool()
        {
            return !empty();
        }
    };
}

#endif