#include <iostream>
#include <vector>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/search/search.h>
#include <pcl/search/kdtree.h>
#include <pcl/features/normal_3d.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/segmentation/region_growing.h>


int
main(int argc, char** argv)
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);
	if (pcl::io::loadPCDFile <pcl::PointXYZ>("3.pcd", *cloud) == -1)
	{
		std::cout << "Cloud reading failed." << std::endl;
		return (-1);
	}
	else 
	{
		std::cout << "Cloud loaded" << std::endl;
	}



	// Create the filtering object
	pcl::VoxelGrid<pcl::PointXYZ> sor;
	sor.setInputCloud(cloud);

	sor.setLeafSize(0.005f, 0.005f, 0.005f);
	sor.filter(*cloud_filtered);
	cloud = cloud_filtered;

	pcl::search::Search<pcl::PointXYZ>::Ptr tree = boost::shared_ptr<pcl::search::Search<pcl::PointXYZ> >(new pcl::search::KdTree<pcl::PointXYZ>);
	pcl::PointCloud <pcl::Normal>::Ptr normals(new pcl::PointCloud <pcl::Normal>);
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normal_estimator;
	normal_estimator.setSearchMethod(tree);
	normal_estimator.setInputCloud(cloud);
	//normal_estimator.setRadiusSearch(0.05);
	normal_estimator.setKSearch(30);
	normal_estimator.compute(*normals);


	pcl::IndicesPtr indices(new std::vector <int>);
	pcl::PassThrough<pcl::PointXYZ> pass;
	pass.setInputCloud(cloud);
	pass.setFilterFieldName("z");
	pass.setFilterLimits(0.0, 1.0);
	pass.filter(*indices);

	pcl::RegionGrowing<pcl::PointXYZ, pcl::Normal> reg;
	reg.setMinClusterSize(5);
	reg.setMaxClusterSize(1000000);
	reg.setSearchMethod(tree);
	reg.setNumberOfNeighbours(10);
	reg.setInputCloud(cloud);
	//reg.setIndices (indices);
	reg.setInputNormals(normals);

	reg.setSmoothnessThreshold(10 / 180.0 * M_PI);
	reg.setCurvatureThreshold(.04);

	std::vector <pcl::PointIndices> clusters;
	reg.extract(clusters);

	std::cout << "Number of clusters is equal to " << clusters.size() << std::endl;
	std::cout << "First cluster has " << clusters[0].indices.size() << " points." << endl;
	std::cout << "These are the indices of the points of the initial" <<
		std::endl << "cloud that belong to the first cluster:" << std::endl;
	int counter = 0;
	while (counter < clusters[0].indices.size())
	{
		std::cout << clusters[0].indices[counter] << ", ";
		counter++;
		if (counter % 10 == 0)
			std::cout << std::endl;
	}
	std::cout << std::endl;

	pcl::PointCloud <pcl::PointXYZRGB>::Ptr colored_cloud = reg.getColoredCloud();
	pcl::visualization::PCLVisualizer viewer("Cluster viewer");
	viewer.setBackgroundColor(150, 150, 150, 0);
	//viewer.showCloud(colored_cloud);
	viewer.addPointCloud(colored_cloud);
	viewer.spin();
	while (!viewer.wasStopped())
	{
	}
	return (0);
}


//void ShowACloud(PointCloud<PointT>::Ptr cloud)
//{
//	pcl::visualization::PCLVisualizer viewer("Simple Cloud Viewer");
//
//	viewer.setBackgroundColor(150, 150, 150, 0);
//
//	viewer.addPointCloud(cloud);
//	viewer.spin();
//	while (!viewer.wasStopped())
//	{
//	}
//}
//
//void ShowACloud(PointCloud<PointT>::Ptr cloud)
//{
//	pcl::visualization::PCLVisualizer viewer("Simple Cloud Viewer");
//
//	viewer.setBackgroundColor(150, 150, 150, 0);
//
//	viewer.addPointCloud(cloud);
//	viewer.spin();
//	while (!viewer.wasStopped())
//	{
//	}
//}