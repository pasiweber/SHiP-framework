Customization and Composition
=============================

Components
----------
- :doc:`available_trees`
- :doc:`available_hierarchies`
- :doc:`available_partitionings`

Each of the above components — trees, hierarchies, and partitioning strategies — can be **independently selected and composed**. This enables flexible experimentation and tailored clustering behavior for a wide range of data types and analysis goals.

Example:

.. code-block:: python

   from SHiP import SHiP
   from SHiP.ultrametric_tree import UltrametricTreeType
   from SHiP.partitioning import PartitioningMethod

   ship = SHiP(data=my_data, treeType=UltrametricTreeType.DCTree)
   labels = ship.fit_predict(hierarchy=2, partitioningMethod=PartitioningMethod.Elbow)
