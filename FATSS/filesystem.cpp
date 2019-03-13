#include "filesystem.h"

FileSystem::FileSystem(int Size)
{
    fat = new QVector<QPair<int, int>>();
    rootDir = new QVector<QPair<QString, int>>();

    size = Size;

    //Size equals clusters

    freeClusters = Size;
    data = new QVector<int>(freeClusters);
}


///
/// \brief FileSystem::format: Prepare the volume for it to be used
///
void FileSystem::format()
{
    //fill the data so we can use replace() later
    fat->fill(QPair<int, int>(0, 1), freeClusters);

}


///
/// \brief FileSystem::createFile: Create a file with the given filename, and size
/// \param Name
/// \param Size
/// \return
///
QString FileSystem::createFile(QString Name, int Size)
{

    //Check if there's enough place
    int neededClusters = Size / clusterSize;
    if( freeClusters < neededClusters )
        return "The volume is full";

    //Check for the first free place
    int index = findFreeCluster(0);
    //Add it to the rootDir
    QPair<QString, int> fileEntry(Name, index);
    rootDir->append(fileEntry);
    //int entryNum = rootDir->indexOf(fileEntry);

    //Create the array containing the indexes of the free clusters to use
    QList<int> clusters;
    clusters[0] = index;
    int clustersIndex = 1;

    //Find all clusters
    for(int i = index; i < fat->length() && clustersIndex < neededClusters; i++)
    {
        if(fat->at(i).first == 0)
        {
            clusters[clustersIndex] = i;
            clustersIndex++;
        }
    }

    //Fill the FAT array with the indexes and insert the last index
    for(int i = 0; i < neededClusters-1; i++)
    {
        fat->insert(clusters[i], QPair<int, int>(1, clusters[i+1]));
    }
    fat->insert(clusters[neededClusters-1], QPair<int, int>(1, -1));

    //Updates the free clusters counter
    freeClusters -= neededClusters;

    //For the signal
    FileEntry* file = new FileEntry(Name, &clusters);
    emit createdFile(file);

    return "File successfully created";
}


///
/// \brief FileSystem::delFile: Delete the file with the given filename.
/// \param name
///
QString FileSystem::delFile(QString name)
{
    //Find file in root directory
    int indexCluster = -1;
    for(int i = 0; i < rootDir->length(); i++)
    {
        if(rootDir->at(i).first == name.toLower())
        {
            indexCluster = i;
        }
    }

    if(indexCluster == -1)
        return "No file with this name";

    //Unset the chaining clusters
    int lastIndex;
    do
    {
        lastIndex = indexCluster;
        indexCluster = fat->at(lastIndex).second;
        fat->replace(lastIndex, QPair<int, int>(0, -1));

    } while(fat->at(indexCluster).second != -1);


    emit deletedFile(name);
    return "file " + name + " has been removed successfully";
}


///
/// \brief FileSystem::findFreeCluster: Find the next free cluster starting at the given index
/// \param start
/// \return
///
int FileSystem::findFreeCluster(int index)
{
    for(int i = index; i < fat->length(); i++)
    {
        if(fat->at(i).first == 0)
        {
            return i;
        }
    }

    return -1;
}
