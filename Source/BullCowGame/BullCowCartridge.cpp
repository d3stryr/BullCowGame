// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"



void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir()/TEXT("WordList/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms,*WordListPath,[](const FString& Word){
        return Word.Len()>=4 && Word.Len()<=8 && IsIsogram(Word);
    });


    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
   
    if(bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::EndGame()
{
    bGameOver=true;
    
    PrintLine(TEXT("\nPress Enter to Play Again "));
}

void UBullCowCartridge::LifeCount()
{
    PrintLine(FString::Printf(TEXT("Guess Again, You have %i Life Left."),Lives));
}
bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    for(int i=0;i<Word.Len();i++)
    {
        for(int j=i+1;j<Word.Len();j++)
        {
            if(Word[i]==Word[j])
            {
                return false;
            }
        }
    }
    return true;
}
void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(Guess==HiddenWord)
    {
        PrintLine(TEXT("Win"));
        EndGame();
        return;
    }


    if(HiddenWord.Len()!=Guess.Len())
    {
        PrintLine(FString::Printf(TEXT("The Hidden Word is %i character long."),HiddenWord.Len()));
        LifeCount();
        return;
    }
    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again"));
        LifeCount();
        return;
    }
    Lives--;
    PrintLine(TEXT("You Lost a Life"));
    if(Lives<=0)
    {
        ClearScreen();
        PrintLine(FString::Printf(TEXT("You have no Lives Left")));
        PrintLine(FString::Printf(TEXT("The Hidden Word was : %s"),*HiddenWord));
        PrintLine(TEXT("Lose"));
        EndGame();
        return;
    }
    int32 Bulls, Cows;
    FBullCowCount BCcount=GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"),BCcount.Bulls, BCcount.Cows);
    LifeCount();

}
TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    for(FString TempVar : WordList)
    {
        if(TempVar.Len()>=4&&TempVar.Len()<=8 && IsIsogram(TempVar))
        {
            ValidWords.Emplace(TempVar);
        }
    }
    return ValidWords;
}
void UBullCowCartridge::SetupGame()
{

    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num()-1)];
    Lives=HiddenWord.Len()*2;
    bGameOver=false;
    PrintLine(TEXT("Welcome To Bull Cows !!!"));   
    PrintLine(FString::Printf(TEXT("Guess the %i Letter Word : "),HiddenWord.Len()));
    PrintLine(TEXT("Type In your Guess and \nPress enter to continue...."));
    PrintLine(FString::Printf(TEXT("\nThe Hidden Word is : %s"),*HiddenWord));   
}
FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount BC;

    for(int32 GuessIndex=0;GuessIndex<Guess.Len();GuessIndex++)
    {
        if(Guess[GuessIndex]==HiddenWord[GuessIndex])
        {
            BC.Bulls++;
            continue;
        }
        for(int32 HiddenIndex=0;HiddenIndex<Guess.Len();HiddenIndex++)
        {
            if(Guess[GuessIndex]==HiddenWord[HiddenIndex])
            {
                BC.Cows++;
                break;
            }
        }

    }
    return BC;
}