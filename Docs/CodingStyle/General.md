# General Code Style Guidelines

## Naming convention
 * For non-public class fields, prefix `m_` is used. first letter after the prefix should be non-capital, but first letter of each following word should be capitalized. Example: `uint32_t m_frameCount;`

 * Functions should start from capital letter, and each word in the name should also start from a capital letter. Example: `void ResetAllObjects();`

 * Comments should not explain what's happening in the section. They should explain why it's happening. Use comments to explain ambiguous or subtle mechanisms of code, not its flow.

 * Self-explanatary code should be prioritized over comments.
    ```c++
    Don't:
    // View into record's field data
    auto data = GetService<DataManager>()->OpenView(m_path);
    
    Do:
    DataView recordFieldsDataView = GetService<DataManager>()->OpenView(m_pluginFilePath);
    ```

    
## Memory Ownership
All code must explicitly state ownership model using C++ smart pointers unless stated otherwise.

### Unique Ownership
If class has complete ownership of object and its lifecycle, `std::unique_ptr` should be used to avoid memory leaks on destruction. 

For consumer of its data to use it, `Raw Pointers` can be used.
```c++
// Data owner
class MyService : public IService {
    std::unique_ptr<Data> m_data;
public:
    Data* GetData() { return m_data.get(); }
};

// Data consumer
void DataConsumer() {
    Data* myData = GetService<MyService>()->GetData();
    if(!myData) {
        // Handle error
    }

    myData->Function();
}
```

### Shared Ownership
If class has shared ownership of data with other elements, `std::shared_ptr` should be used. In this case, resource's lifetime will be shared between all objects storing the pointer.

### Observing Ownership
Sometimes we don't want a single class to own and manage data and its lifecycle. Instead we want consumers to grab and use it, and our class to know when it's not used by anything anymore(aka being an `Observer`). Here we can use `std::weak_ptr` or `std::shared_ptr` based on whether we want deletion immediately after references are released or deferred to later. 

```C++
// Data owner
class DataView {
    // Owns the reference, decrementing counter on destructor
    std::shared_ptr<Data> m_data;
public:
    DataView(std::shared_ptr<Data> && data) : m_data(data) {}
};

// Data observer
class ObserverService : public IService {
    // Only observes the reference, doesn't influence the counter
    std::weak_ptr<Data> m_data;
public:
    DataView LoadData() {
        if(!m_data.expired()) {
            return DataView(m_data.lock()); 
        } else {
            // Load data and return DataView
            std::shared_ptr<Data> sharedData = std::make_shared<Data>();
            m_data = sharedData;
            return DataView(std::move(sharedData));
        }
    }
};

// Data consumer
void DataConsumer() {
    DataView myData = GetService<ObserverService>()->LoadData();
    if(!myData) {
        // Handle error
    }

    myData->Function();
}
```
This way, once all references(DataViews holding std::shared_ptr) to data are destroyed, memory will also be removed and ObserverService will know about it, hence loading data again if required.