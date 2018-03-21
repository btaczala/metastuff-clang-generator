#include <iostream>

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include <fmt/format.h>

namespace {
const std::string kDefaultFileTemplate = R"(
#ifndef {INCLUDE_GUARD_NAME}
#define {INCLUDE_GUARD_NAME}

#include <Meta.h>

namespace meta {{
{META_IMPL}
}}
#endif {INCLUDE_GUARD_NAME}
)";

const std::string kDefaultSpecializationTemplate =
    // clang-format off
R"(inline auto registerMembers<{TYPE}>() {{
    return members(
{META_MEMBERS}
    )}};
)";
// clang-format on

const std::string kDefaultMemberTemplate =
    R"(member("{SHORT_MEMBER_NAME}", &{MEMBER_NAME}))";

struct Struct {
    std::string name;
    std::string memberTemplatesFilled;
};

std::vector<Struct> data;
}  // namespace

class StructDeclASTVisitor
    : public clang::RecursiveASTVisitor<StructDeclASTVisitor> {
    clang::SourceManager& sourceManager_;

   public:
    explicit StructDeclASTVisitor(clang::SourceManager& sm)
        : sourceManager_(sm) {}

    bool VisitCXXRecordDecl(clang::CXXRecordDecl* decl) {
        if (sourceManager_.isWrittenInMainFile(
                decl->getSourceRange().getBegin())) {
            Struct s;
            s.name = decl->getQualifiedNameAsString();

            const auto fields = decl->fields();
            std::string buff;

            std::for_each(
                std::begin(fields), std::end(fields), [&buff](const auto& f) {
                    using namespace fmt::literals;
                    const auto shortMemberName = f->getNameAsString();
                    const auto memberName = f->getQualifiedNameAsString();
                    buff += "        ";
                    buff += fmt::format(kDefaultMemberTemplate,
                                        "SHORT_MEMBER_NAME"_a = shortMemberName,
                                        "MEMBER_NAME"_a = memberName);
                    buff += ",\n";
                });

            buff = buff.substr(0, buff.size() - 2);
            s.memberTemplatesFilled = buff;
            data.push_back(s);
        }
        return true;
    }
};

class StructDeclASTConsumer : public clang::ASTConsumer {
    StructDeclASTVisitor visitor_;  // doesn't have to be private

   public:
    // override the constructor in order to pass CI
    explicit StructDeclASTConsumer(clang::CompilerInstance& ci)
        : visitor_(ci.getSourceManager()) {}

    virtual void HandleTranslationUnit(clang::ASTContext& astContext) {
        visitor_.TraverseDecl(astContext.getTranslationUnitDecl());
    }
};

class StructDeclFrontendAction : public clang::ASTFrontendAction {
   public:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& CI, clang::StringRef file) {
        return std::make_unique<StructDeclASTConsumer>(
            CI);  // pass CI pointer to ASTConsumer
    }
};

int main(int argc, const char* argv[]) {
    llvm::cl::OptionCategory ms_generator{"metastuff-generator options"};

    clang::tooling::CommonOptionsParser opts{argc, argv, ms_generator};

    clang::tooling::ClangTool tool{opts.getCompilations(),
                                   opts.getSourcePathList()};

    auto ec = tool.run(
        clang::tooling::newFrontendActionFactory<StructDeclFrontendAction>()
            .get());
    std::string buff;
    for (const auto& oneEntry : data) {
        using namespace fmt;
        buff += fmt::format(kDefaultSpecializationTemplate,
                            "TYPE"_a = oneEntry.name,
                            "META_MEMBERS"_a = oneEntry.memberTemplatesFilled);
        buff += "\n";
    }

    using namespace fmt;
    std::cout << fmt::format(kDefaultFileTemplate,
                             "INCLUDE_GUARD_NAME"_a = "TEST",
                             "META_IMPL"_a = buff)
              << std::endl;

    return ec;
}
