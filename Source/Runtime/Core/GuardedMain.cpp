#include <Application/Application.h>
#include <Platform/Platform.h>

// Dev
#include <GarbageCollector/GarbageCollector.h>
#include <Record/RecordLibrary.h>
#include <Assets/AssetLibrary.h>
#include <GameFramework/Records/TextureRecord/TextureRecord.h>
#include <GameFramework/Records/AtlasRecord/AtlasRecord.h>

int GuardedMain(int argc, char* argv[])
{
    Application application;

    application.Initialize();

    LOG(Log, LogGuardedMain, "Starting application life cycle");

    // Test
    //AssetPtr<RawAsset> asset = GetService<AssetLibrary>()->LoadRawAsset("./Data/Image.png");

    RecordPtr<TextureRecord> albedoRec = GetService<RecordLibrary>()->CreateRecord<TextureRecord>();
    albedoRec->EditorID = "Albedo";
    /* TODO: Make absolute resolving ("Texture/Image.png" -> "E:/Game/Data/Texture/Image.png")*/
    albedoRec->TexturePath = "./Data/Image.png";

    RecordPtr<Record> atlasRec = GetService<RecordLibrary>()->CreateRecordFromType(AtlasRecord::StaticType());
    atlasRec->EditorID = "Atlas";
    ((AtlasRecord*)atlasRec.Get())->AlbedoTexture = albedoRec.GetID();

    RecordPtr<ReferenceRecord> refRec = GetService<RecordLibrary>()->CreateRecord<ReferenceRecord>();
    refRec->EditorID = "SpriteReference";
    refRec->BaseTypename = atlasRec->GetType();
    refRec->Base = atlasRec->GetID();
    refRec->IsDynamic = true;

    std::unique_ptr<World> myWorld = std::make_unique<World>();

    ActorInstantiateInfo acinfo1{};
    Actor* act1 = myWorld->InstantiateActor(refRec.Get(), acinfo1);

    ActorCreateInfo acinfo2{};
    acinfo2.transform.Location = {32, 0};
    acinfo2.base = atlasRec.Get(); 
    acinfo2.displayName = Text("Test.Actor.Name"); 
    Actor* act2 = myWorld->SpawnActor(acinfo2);

    application.GetEngine()->TravelTo(std::move(myWorld));
    // This should trigger GC
    GetService<GarbageCollector>()->RequestGCPass();

    application.StartLifecycle();

    return 0;
}