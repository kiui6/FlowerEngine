#include "Record.h"

#include "RecordLibrary.h"
#include <Log/Log.h>

void Record::RequestSelfDestruction()
{
    RecordLibrary* library = GetService<RecordLibrary>();
    if(library) {
        library->UnloadRecord(m_id);
    } else {
        LOG(Assert, LogAsset, "Attempted to destroy Record, but RecordLibrary service doesn't exist");
    }
}

bool Record::IsDirty()
{
    if(HasFlag(RecordFlags::Dirty)) {
        return true;
    }

    // TODO: What whould we do about it?
    //for(FieldBase* field : this->GetFields()) {
    //    if(field->IsDirty()) {
    //        // Set record dirty so that consequential calls are resolved faster
    //        SetFlag(RecordFlags::Dirty);
    //        return true;
    //    }
    //}

    return false;
}

bool Record::Serialize(RecordObject &object)
{
    object
    .CreateFieldObject()
    .CreateField(EditorID.GetID(), FieldNodeType::String)
    .SetString(EditorID.Get());
    return false;
}

bool Record::Deserialize(const RecordObject &object)
{
    RecordFieldObject* fieldObject = object.GetFinal();
    if(!fieldObject) {
        return false;
    }
    
    for(FieldBase* field : this->GetFields()) {
        if(FieldNode* node = fieldObject->GetField(field->GetID())) {
            field->Deserialize(node);
        }
    }

    return true;
}
