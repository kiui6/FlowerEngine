#pragma once

class IAsyncTask {
public:
    virtual ~IAsyncTask() {}
    virtual void Execute() {}
};