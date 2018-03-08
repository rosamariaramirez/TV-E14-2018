#pragma once

#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "JsonUtilities.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpService.generated.h"

USTRUCT()
struct FGERequest
{
    GENERATED_BODY()
    UPROPERTY()
    FString name;
    UPROPERTY()
    FString lastName;
    
    FGERequest() {
        name = "";
        lastName = "";
    }
};
USTRUCT()
struct FGERequest_Response
{
    GENERATED_BODY()
    UPROPERTY()
    FString message;
    
    FGERequest_Response() {
        message = "";
    }
};

UCLASS(Blueprintable, hideCategories = (Rendering, Replication, Input, Actor, "Actor Tick"))
class GOOGLEVR_TEST_02_API AHttpService : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
    FHttpModule* Http;
    FString APIBaseUrl = "http://localhost:8080/api/";
    
    FString AuthorizationHeader = TEXT("Authorization");
    void SetAuthorizationHash(FString Hash, TSharedRef<IHttpRequest>& Request);
    
    TSharedRef<IHttpRequest> RequestWithRoute(FString subroute);
    void SetRequestHeaders(TSharedRef<IHttpRequest>& Request);
    
    TSharedRef<IHttpRequest> PostRequest(FString Subroute, FString ContentJSONString);
    void Send(TSharedRef<IHttpRequest>& Request);
    
    bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);
    
    template <typename StructType>
    void GetJSONStringFromStruct(StructType FilledRequest, FString& StringOutput);
    template <typename StructType>
    void GetStructFromJSONString(FHttpResponsePtr Response, StructType& StructOutput);
    
public:
    // Sets default values for this actor's properties
    AHttpService();
    void PostSession(FGERequest SessionInformation);
    
    void SaveSession(FGERequest FilledSession);
    void SaveSessionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
