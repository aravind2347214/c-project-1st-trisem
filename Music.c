#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>

typedef enum {POP, ROCK, JAZZ, HIPHOP} Genre;

typedef struct {
    char title[100];
    char artist[100];
    char album[100];
    Genre genre;
} Song;

typedef struct {
    Song *songs;
    int numSongs;
} SongDatabase;

void addSong(SongDatabase *database);
void displayAllSongs(SongDatabase *database);
void searchSong(SongDatabase *database, char *title);
void removeSong(SongDatabase *database, char *title);
void bubbleSortByTitle(Song *arr, int numSongs);
void bubbleSortByArtist(Song *arr, int numSongs);
int binarySearchByTitle(Song *arr, int left, int right, char *title);
void saveToFile(SongDatabase *database, const char *filename);
void loadFromFile(SongDatabase *database, const char *filename);

int main() {
    SongDatabase database;
    database.numSongs = 0;
    database.songs = NULL;

    // Load existing song records from file
    loadFromFile(&database, "songs.txt");
    

    int choice;
    do {
    	 
        printf("\n +----------------Menu----------------+:\n");
        printf(" |  1. Add a song                     |\n");
        printf(" |  2. Display all songs              |\n");
        printf(" |  3. Search for a song by title     |\n");
        printf(" |  4. Remove a song by title         |\n");
        printf(" |  5. Sort songs by title            |\n");
        printf(" |  6. Sort songs by artist           |\n");
        printf(" |  7. Save songs to file             |\n");
        printf(" |  8. Exit                           |\n");
        printf(" +------------------------------------+:\n");
        printf(" Enter your choice: ");

        // Get user choice
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addSong(&database);
                break;
            case 2:
                displayAllSongs(&database);
                break;
            case 3:
                {
                    char searchTitle[100];
                    printf("Enter song title to search for: ");
                    scanf(" %[^\n]", searchTitle);
                    searchSong(&database, searchTitle);
                }
                break;
            case 4:
                {
                    char removeTitle[100];
                    printf("Enter song title to remove: ");
                    scanf(" %[^\n]", removeTitle);
                    removeSong(&database, removeTitle);
                }
                break;
            case 5:
                  bubbleSortByTitle(database.songs, database.numSongs);
                  printf("Songs sorted by title.\n");
                     break;
              case 6:
                  bubbleSortByArtist(database.songs, database.numSongs);
                printf("Songs sorted by artist.\n");
                  break;
            case 7:
                // Save the song database to file
                saveToFile(&database, "songs.txt");
                printf("Songs saved to file.\n");
                break;
            case 8:
                // Exit the program
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);

    // Free dynamically allocated memory

    return 0;
}


void addSong(SongDatabase *database) {
    // Increment the number of songs in the database
    database->numSongs++;

    // Allocate memory for the new song
    database->songs = (Song *)realloc(database->songs, database->numSongs * sizeof(Song));

    if (database->songs == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Fill in the details for the new song
    Song *newSong = &database->songs[database->numSongs - 1];

    printf("Enter song title: ");
    scanf(" %[^\n]", newSong->title);

    printf("Enter artist: ");
    scanf(" %[^\n]", newSong->artist);

    printf("Enter album: ");
    scanf(" %[^\n]", newSong->album);

    printf("Enter genre (0: Pop, 1: Rock, 2: Jazz, 3: HipHop): ");
    int genre;
    scanf("%d", &genre);

    if (genre >= 0 && genre <= 3)
        newSong->genre = (Genre)genre;
    else {
        printf("Invalid genre. Setting to Pop.\n");
        newSong->genre = POP;
    }
}



void displayAllSongs(SongDatabase *database) {
    printf("Song Records:\n");
    int i;

    for (i = 0; i < database->numSongs; i++) {
        printf("Title: %s, Artist: %s, Album: %s, Genre: %d\n",database->songs[i].title, database->songs[i].artist,database->songs[i].album, database->songs[i].genre);
    }
}


void searchSong(SongDatabase *database, char *title) {
    int result = binarySearchByTitle(database->songs, 0, database->numSongs - 1, title);

    if (result == -1)
        printf("Song with title \"%s\" not found.\n", title);
    else
        printf("Song found: Title: %s, Artist: %s, Album: %s, Genre: %d\n",
               database->songs[result].title, database->songs[result].artist,
               database->songs[result].album, database->songs[result].genre);
}


void removeSong(SongDatabase *database, char *title) {
    int index = binarySearchByTitle(database->songs, 0, database->numSongs - 1, title);

    if (index == -1) {
        printf("Song with title \"%s\" not found.\n", title);
        return;
    }

    // Move the last song to the position of the song to be removed
    database->songs[index] = database->songs[database->numSongs - 1];
    database->numSongs--;

    // Reallocate memory to adjust for the removed song
    database->songs = (Song *)realloc(database->songs, database->numSongs * sizeof(Song));

    if (database->songs == NULL) {
        printf("Memory reallocation failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("Song with title \"%s\" removed successfully.\n", title);
}


void saveToFile(SongDatabase *database, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

int i;
    for ( i= 0; i < database->numSongs; i++) {
        fprintf(file, "%s,%s,%s,%d\n", database->songs[i].title,
                database->songs[i].artist, database->songs[i].album, database->songs[i].genre);
    }

    fclose(file);
}


void loadFromFile(SongDatabase *database, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    Song song;
    while (fscanf(file, "%[^,],%[^,],%[^,],%d\n", song.title, song.artist, song.album, &song.genre) == 4) {
        database->numSongs++;
        database->songs = (Song *)realloc(database->songs, database->numSongs * sizeof(Song));

        if (database->songs == NULL) {
            printf("Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        database->songs[database->numSongs - 1] = song;
    }

    fclose(file);
}


int binarySearchByTitle(Song *arr, int left, int right, char *title) {
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(arr[mid].title, title);

        if (cmp == 0)
            return mid;

        if (cmp < 0)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1;  // Not found
}


void swapSong(Song *a, Song *b) {
    Song temp = *a;
    *a = *b;
    *b = temp;
}



void bubbleSortByArtist(Song *arr, int numSongs) {
	int i;
    for ( i = 0; i < numSongs - 1; i++) {
    	int j;
        for ( j = 0; j < numSongs - i - 1; j++) {
            if (strcmp(arr[j].artist, arr[j + 1].artist) > 0) {
                swapSong(&arr[j], &arr[j + 1]);
            }
        }
    }
}


void bubbleSortByTitle(Song *arr, int numSongs) {
	int i;
    for ( i = 0; i < numSongs - 1; i++) {
    	int j;
        for ( j = 0; j < numSongs - i - 1; j++) {
            if (strcmp(arr[j].title, arr[j + 1].title) > 0) {
                swapSong(&arr[j], &arr[j + 1]);
            }
        }
    }
}
