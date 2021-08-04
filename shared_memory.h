#pragma once

#include <string>
#include <mutex>
#include <iostream>

namespace SM {

    class AbsSharedMemory {

    public:

        //-----------------------------------
        AbsSharedMemory() = default;

        virtual ~AbsSharedMemory() = default;

        //-----------------------------------

        virtual std::string &GetSharedMemory() = 0;

        //-----------------------------------

        virtual void SetSharedMemory(const char *, std::size_t) = 0;

        //-----------------------------------

        virtual void PrintSharedMemory() const = 0;

        //-----------------------------------

    };

    class SharedMemory : public AbsSharedMemory {

    public:
        //-----------------------------------

        SharedMemory() = default;

        //-----------------------------------

        std::string &GetSharedMemory() override;

        //-----------------------------------

        void SetSharedMemory(const char *, std::size_t) override;

        //-----------------------------------

        void PrintSharedMemory() const override;

        //-----------------------------------

    private:
        std::string shared_memory{"start"};
    };


} //namespace SM