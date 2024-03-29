#pragma once

#include <bier/core/layout.h>
#include <bier/core/operation.h>
#include <bier/operations/opcodes.h>

namespace bier {

class AllocateLayout : public BaseOperation<OpCodes::ALLOC_LAYOUT_OP> {
public:
    AllocateLayout(const Function* context, const Layout* layout, const Value* count,
                   const Variable* result);

    // Интерфейс Operation
    std::vector<const Value*> GetArguments() const override;
    void SubstituteArguments(const std::vector<const Value*>& args) override;
    std::optional<const Variable*> GetReturnValue() const override;
    void SubstituteReturnValue(const Variable* return_value) override;

    // Интерфейс FunctionContextMember
    const Function* GetContextFunction() const override {
        return context_;
    }

    const Layout* GetLayout() const {
        return layout_;
    }

private:
    const Function* context_ = nullptr;
    const Layout* layout_ = nullptr;
    const Value* count_ = nullptr;
    const Variable* result_ptr_ = nullptr;
};

}  // namespace bier
