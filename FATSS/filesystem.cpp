#include "filesystem.h"

FileSystem::FileSystem(int Size)
{
    rootDir = new QVector<QPair<QString, int>>();
    this->fat = new QVector<FatEntry*>();
    for(int i = 0; i < Size; i++)
    {
        fat->insert(i, new FatEntry());
    }
    size = Size;
    freeClusters = Size;
}

///
/// \brief FileSystem::createFile: Create a file with the given filename, and size
/// \param Name
/// \param Size
/// \return
///
QString FileSystem::createFile(QString name, int Size)
{

    //Check if there's enough place
    int neededClusters = Size;
    if( freeClusters < neededClusters )
        return "The volume is full";

    //Check for the first free place
    int index = findFreeCluster(0);
    //Add it to the rootDir
    QPair<QString, int> fileEntry(name, index);
    rootDir->append(fileEntry);

    //Create the array containing the indexes of the free clusters to use
    QList<int>* clusters = new QList<int>();
    clusters->append(index);
    int lastClusters = index+1;

    //Find all free clusters
    while(clusters->size() < neededClusters)
    {
        lastClusters = findFreeCluster(lastClusters);
        clusters->append(lastClusters++);
    }

    //Fill the FAT array with the indexes and insert the last index
    for(int i = 0; i < clusters->size()-1; i++)
    {
        fat->at(clusters->at(i))->setVals(clusters->at(i+1), 1);
    }
    fat->at(clusters->at(neededClusters-1))->setVals(-1, 1);

    //Updates the free clusters counter
    freeClusters -= neededClusters;
     qDebug() << "clusters in fs: ";
    for(int i = 0; i< clusters->size(); i++)
    {
        qDebug() << " , " << clusters->at(i);
    }

    //For the signal
    FileEntry* file = new FileEntry(name, clusters);
    emit createdFile(file);
    qDebug() << "EMITTED";
    return "File successfully created";
}


QString FileSystem::updateFile(QString name, int newSize)
{
    int indexFile = -1;
    for(int i = 0; i < rootDir->length(); i++)
    {
        if(rootDir->at(i).first == name.toLower())
        {
            indexFile = i;
        }
    }

    if(indexFile == -1)
        return "No file with this name";
    int indexCluster = rootDir->at(indexFile).second;

    //Run through the existing size of the file
    //If the Newsize is smaller than the actual size -> continue and unset the clusters
    //that are beyond the new size until reaching a cluster that link to "-1"

    //If the NewSize is larger than the actual size -> after the size is run through,
    //continue adding clusters to the chain until the NewSize is attained

    bool isLarger = false;
    int size;

    int lastCluster;

    QList<int> *clusters = new QList<int>();
    // Check if it's a reduction or an inflation
    for(size = 1; size <= newSize; size++) //Run through while we haven't reached the newSize
    {
        clusters->append(indexCluster);
        //indexCluster is the pointer on the current cluster in the fat
        if(fat->at(indexCluster)->nextEntry ==  -1)
        {
            //We go here if we haven't reached the newSize and the size is already reached
            //It means the actual size is smaller than NewSize, so inflation
            isLarger = true;
            break;
        }
        lastCluster = indexCluster;
        indexCluster = fat->at(indexCluster)->nextEntry;
    }//If we finish the for loop without entering the condition, it means the size is at least equal to the NewSize, so it's a reduction



    //Inflation
    if(isLarger)
    {
        //Check if the newsize is within the free clusters
        if(freeClusters < newSize-size )
            return "Not enough space left";
        freeClusters -= newSize - size;


        int nextCluster;
        //While the size is smaller than the newsize, we keep adding to the chain
        while(size < newSize)
        {
            //Find next cluster
            //Link current to next cluster
            //update next cluster to used
            //add next cluster to used cluster

            //Find next cluster
            nextCluster = findFreeCluster(indexCluster);
            clusters->append(nextCluster);

            //update current cluster
            fat->at(indexCluster)->nextEntry = nextCluster;

            //Update next cluster
            fat->at(nextCluster)->setVals(-1, 1);

            indexCluster = nextCluster;
            size++;
        }
    }
    else //Reduction
    {
        fat->at(lastCluster)->nextEntry = -1;
        //While we're not at the end of the file, unset the clusters' nextentry
        while(indexCluster != -1)
        {
            fat->at(indexCluster)->state = 0;
            indexCluster = fat->at(indexCluster)->nextEntry;
        }
    }

    FileEntry* file = new FileEntry(name, clusters);
    emit updatedFile(file);
    return "Done";
}

///
/// \brief FileSystem::delFile: Delete the file with the given filename.
/// \param name
///
QString FileSystem::delFile(QString name)
{
    //Find file in root directory
    int indexFile = -1;
    for(int i = 0; i < rootDir->length(); i++)
    {
        if(rootDir->at(i).first == name.toLower())
        {
            indexFile = i;
        }
    }

    if(indexFile == -1)
        return "No file with this name";
    int indexCluster = rootDir->at(indexFile).second;
    rootDir->remove(indexFile);
    int sizeFile = 0;
    //Unset the chaining clusters
    int lastIndex;
    do
    {
        lastIndex = indexCluster;
        indexCluster = fat->at(lastIndex)->nextEntry;
        fat->at(lastIndex)->state = 0;
        sizeFile++;
    } while(indexCluster != -1);

    freeClusters += sizeFile;
    emit deletedFile(name);
    return "file " + name + " has been removed successfully";
}

FileSystem::~FileSystem()
{
    qDeleteAll(*fat);
    delete fat;
}


///
/// \brief FileSystem::findFreeCluster: Find the next free cluster starting at the given index
/// \param start
/// \return
///
int FileSystem::findFreeCluster(int index)
{
    for(int i = index; i < fat->count(); i++)
    {
        if(fat->at(i)->state == 0)
        {
            return i;
        }
    }


    return -1;
}
