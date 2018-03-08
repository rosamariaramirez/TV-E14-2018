#include "HttpService.h"

// Sets default values
AHttpService::AHttpService()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AHttpService::BeginPlay()
{
	Super::BeginPlay();
    Http = &FHttpModule::Get();
    
    FGERequest PostRequestStruct;
    PostRequestStruct.name = TEXT("Juan M");
    PostRequestStruct.lastName = TEXT("Romero");
    
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "debug msg");
    
    SaveSession(PostRequestStruct);
}

TSharedRef<IHttpRequest> AHttpService::RequestWithRoute(FString Subroute) {
    TSharedRef<IHttpRequest> Request = Http->CreateRequest();
    Request->SetURL(APIBaseUrl + Subroute);
    SetRequestHeaders(Request);
    return Request;
}

void AHttpService::SetRequestHeaders(TSharedRef<IHttpRequest>& Request) {
    Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
}

TSharedRef<IHttpRequest> AHttpService::PostRequest(FString Subroute, FString ContentJsonString) {
    TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
    Request->SetVerb("POST");
    Request->SetContentAsString(ContentJsonString);
    return Request;
}

void AHttpService::Send(TSharedRef<IHttpRequest>& Request) {
    Request->ProcessRequest();
}

bool AHttpService::ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful) {
    if (!bWasSuccessful || !Response.IsValid()) return false;
    if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) return true;
    else {
        UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code: %d"), Response->GetResponseCode());
        return false;
    }
}

void AHttpService::SetAuthorizationHash(FString Hash, TSharedRef<IHttpRequest>& Request) {
    Request->SetHeader(AuthorizationHeader, Hash);
}


template <typename StructType>
void AHttpService::GetJSONStringFromStruct(StructType FilledStruct, FString& StringOutput) {
    FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(), &FilledStruct, StringOutput, 0, 0);
}

template <typename StructType>
void AHttpService::GetStructFromJSONString(FHttpResponsePtr Response, StructType& StructOutput) {
    StructType StructData;
    FString JsonString = Response->GetContentAsString();
    FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutput, 0, 0);
}

void AHttpService::SaveSession(FGERequest FilledSession) {
    FString ContentJsonString;
    GetJSONStringFromStruct<FGERequest>(FilledSession, ContentJsonString);
    
    TSharedRef<IHttpRequest> Request = PostRequest("session/", ContentJsonString);
    Request->OnProcessRequestComplete().BindUObject(this, &AHttpService::SaveSessionResponse);
    Send(Request);
}

void AHttpService::SaveSessionResponse( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
    if (!ResponseIsValid(Response, bWasSuccessful)) return;
    
    FGERequest_Response SessionPostResponse;
    GetStructFromJSONString<FGERequest_Response>(Response, SessionPostResponse);
    
    UE_LOG(LogTemp, Warning, TEXT("message is: %s"), *SessionPostResponse.message);
    
}

