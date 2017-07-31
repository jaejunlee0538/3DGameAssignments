Utility SplitMesh "AseImport" silentErrors:false
(
	group "Ase mesh import"
	(
		button aseimp "Import" tooltip:"Import ase mesh"
	)
	on aseimp pressed do
	(
		inipath = scriptsPath + "aseimp.ini"
		filename = getINISetting inipath "settings" "imppath"
	   	meshFilename = getOpenFileName caption:"Select ase file" \
					filename:filename \
					types:"ase file (*.ase)|*.ase|All Files (*.*)|*.*|"

		if meshFilename != undefined then
		(
			setINISetting inipath "settings" "imppath" meshFilename

			local ASE = ASE_LOADER()
			ASE.Open meshFilename
			ASE.LoadAse()
			ASE.CloseAse()
			
			max views redraw
		)
	)
)
struct ASE_MATERIAL
(
	fn read _asefile=
	(
		--skip materials.
		for i = 1 to 15 do
		(
			tok = filterString (readLine _asefile) " \t"
		)
	)
)
struct ASE_TRANSFORM
(
	fn read _asefile=
	(
		--skip nodetransform
		for i = 1 to 10 do
		(
			readLine _asefile
			--format "%\n" ( readLine _asefile )
		)
	)
)
struct ASE_MESH
(
	_timevalue, -- number of frames for the object (vertex animation)
	
	_numverts,
	_numtverts,
	_numfaces,
	_numtfaces,
	_comment,
	
	_vertlist=#(),
	_tvertlist=#(),
	_facelist=#(),
	_tfacelist=#(),
	
	_facenormals=#(),
	_vertexnormals=#(),
	
	fn read _asefile=
	(
		--format "reading mesh\n"
		tok = filterString (readLine _asefile) " \t"
		while tok[1] != "}" do
		(
			--format "%\n" tok[1]
			if tok[1] == "*TIMEVALUE" do
			(
				_timevalue = tok[2] as integer --expect 0
			)
			if tok[1] == "*COMMENT" do
			(
				--eek
			)
			if tok[1] == "*MESH_NUMVERTEX" do
			(
			 	_numverts = tok[2] as integer
			)
			if tok[1] == "*MESH_NUMFACES" do
			(
				_numfaces = tok[2] as integer
			)
			if tok[1] == "*MESH_NUMTVERTEX" do
			(
				_numtverts = tok[2] as integer
			)
			if tok[1] == "*MESH_NUMTVFACES" do
			(
				_numtfaces = tok[2] as integer
			)
			if tok[1] == "*MESH_VERTEX_LIST" do
			(
				for i = 1 to _numverts do
				(
					tok = filterString (readLine _asefile) " \t"
					append _vertlist [tok[3] as float, tok[4] as float, tok[5] as float]
				)
				-- get the "}"
				readLine _asefile
			)
			if tok[1] == "*MESH_NORMALS" do
			(
				format "numfaces: %\n numverts: %\n" _numfaces _numverts
				for i = 1 to _numfaces do
				(
					readLine _asefile
				)
				for i = 1 to _numverts do
				(
					readLine _asefile
				)
				readLine _asefile -- grab "}"
			)
			if tok[1] == "*MESH_FACE_LIST" do
			(
				for i = 1 to _numfaces do
				(
					tok = filterString (readLine _asefile) " \t"
					append _facelist [tok[4] as integer +1 , tok[6] as integer +1 , tok[8] as integer + 1]
				)
				-- get the "}"
				readLine _asefile
			)
			if tok[1] == "*MESH_TVERTLIST" do
			(
				for i = 1 to _numtverts do readLine _asefile
				readLine _asefile
			)
			if tok[1] == "*MESH_TFACELIST" do
			(
				for i = 1 to _numtfaces do readLine _asefile
				readLine _asefile
			)
			tok = filterString (readLine _asefile) " \t"
		)
	)
)
struct ASE_GEOMOBJECT
(
	_node_name,
	_node_transform,
	_mesh,
	
	--properties
	_motionblur,
	_castshadow,
	_receiveshadow,
	_material,
	
	fn read _asefile=
	(
		format "Reading geomobject...\n"
		tok = filterString (readLine _asefile) " \t" -- read *GEOMOBJECT
		while tok[1] != "}" do
		(
			--format "tok1: %\n" tok[1]
			if tok[1] == "*NODE_NAME" do
			(
				_node_name = tok[2]
			)
			if tok[1] == "*NODE_TM" do
			(
				_node_transform = ASE_TRANSFORM
				_node_transform.read _asefile
			)
			if tok[1] == "*PROP_MOTIONBLUR" do _motionblur = tok[2] as integer
			if tok[1] == "*PROP_CASTSHADOW" do _castshadow = tok[2] as integer
			if tok[1] == "*PROP_RECVSHADOW" do _receiveshadow = tok[2] as integer
			if tok[1] == "*MATERIAL_REF" do _material = tok[2] as integer
		
			if tok[1] == "*MESH" do 
			(
				format "found mesh\n"
				_mesh = ASE_MESH()
				_mesh.read _asefile	
			)
			tok = filterString (readLine _asefile) " \t"
		)
		--readLine _asefile -- grab last "}"
	)
)
struct ASE_SCENE
(
	_filename,
	_firstframe,
	_lastframe,
	_framespeed,
	_ticksperframe,
	_backgroundstatic,
	_ambientstatic,

	fn read _asefile=
	(
		tok = filterString (readLine _asefile) "\t"
		--format "tok111: %\n" tok[1]
		if tok[1] == "*SCENE" do
		(
			--format "scene\n"
			while tok[1] != "}" do
			(
				tok = filterString (readLine _asefile) "\t"
				--format "tok1: %\n" tok[1]
				if tok[1] == "*SCENE_FILENAME" do
				(
					_filename = tok[2]
				)
				if tok[1] == "*SCENE_FIRSTFRAME" do
				(
					_firstframe = tok[2] as integer
				)
				if tok[1] == "*SCENE_LASTFRAME" do
				(
					_lastframe = tok[2] as integer
				)
				if tok[1] == "*SCENE_FRAMESPEED" do
				(
					_framespeed = tok[2] as integer
				)
				if tok[1] == "*SCENE_TICKSPERFRAME" do
				(
					_ticksperframe = tok[2] as integer
				)
				if tok[1] == "*SCENE_BACKGROUND_STATIC" do
				(
					_backgroundstatic = [tok[2] as float, tok[3] as float, tok[4] as float]
				)
				if tok[1] == "*SCENE_AMBIENT_STATIC" do
				(
					_ambientstatic = [tok[2] as float, tok[3] as float, tok[4] as float]
				)
			)
		)
	),
	fn debugout=
	(
		format "===ASE scene===\n"
		format "filename: %\n" _filename
		format "firstframe: % lastframe: %\n" _firstframe _lastframe
		format "framespeed: % ticksperframe: %\n" _framespeed _ticksperframe
		--format "background static: %, %, %\n" _backgroundstatic[1] _backgroundstatic[2] _backgroundstatic[3]
		--format "ambient static: %, %, %\n" _ambientstatic[1] _ambientstatic[2] _ambientstatic[3]
	)
)
struct ASE_LOADER
(
	_asefile,
	
	--node info
	_nodename,
	_materialcount,
	
	--ase file data
	_scene,
	_materials=#(),
	_geomobjects=#(),
	
	fn Open filename=
	(
		_asefile = openFile filename mode:"rt"
	),
	fn CloseAse=
	(
		close _asefile
	),
	fn readMaterials = 
	(
		format "Reading materials ..."
		-- material list
		tok = filterString (readLine _asefile) " \t"
	
		while(tok[1] != "}") do
		(	
			if(tok[1] == "*MATERIAL_COUNT") do
			(
				_materialcount = tok[2] as integer
				format "Expecting % materials\n" _materialcount
			)
			if(tok[1] == "*MATERIAL") do
			(

				local material = ASE_MATERIAL()
				material.read _asefile
				append _materials (copy material)
			)
				
			tok = filterString (readLine _asefile) " \t"
		)	
	),
	fn readObject =
	(
		format "Reading object ...\n"
		
		local obj = ASE_GEOMOBJECT()
		obj.read _asefile
		append _geomobjects (copy obj)
	),
	fn LoadAse=
	(
		format "Loading: %\n" _asefile
		
		tok = filterString (readLine _asefile) " \t"

		-- check version
		if tok[1] == "*3DSMAX_ASCIIEXPORT" do 
		(
			if tok[2] != "200" do
			(
				format "Incorrect ase version.\n"
				return false
			)
		)
		
		_scene = ASE_SCENE()
		
		
		while (eof _asefile) == false do
		(
			tok = filterString (readLine _asefile) " \t"
			if(tok[1] == "*COMMENT") do
			(
				format "Comment: "
				for i = 2 to tok.count do
				(
					format "% " tok[i]
				)
				format "\n"
				tok = filterString (readLine _asefile) " \t"
			)
			 if (tok[1] == "*MATERIAL_LIST") do
			(
				readMaterials()
			)
			if(tok[1] == "*GEOMOBJECT") do
			(
					readObject()
			)
		)
		
		
		--_scene.read _asefile
		--_scene.debugout()
			
			
		
		--progressStart("Creating objects...")
		progressStart("Combining objects...")

		progressUpdate(0)
		local vertarray=#()
		local facearray=#()
		local offset = 0
		for i = 1 to _geomobjects.count do
		(
			if ( mod i 100 ) == 0 do progressUpdate( 100.0 * i / _geomobjects.count )
			-- create a mesh for each object
			-- get array of verts
			--local vertarray = _geomobjects[i]._mesh._vertlist 
			--local facearray = _geomobjects[i]._mesh._facelist
			for k = 1 to _geomobjects[i]._mesh._vertlist.count do
			(
				append vertarray _geomobjects[i]._mesh._vertlist[k]
			)
			for k = 1 to _geomobjects[i]._mesh._facelist.count do
			(
				append facearray ( _geomobjects[i]._mesh._facelist[k] + offset )
			)
			offset+= _geomobjects[i]._mesh._vertlist.count
		)
		new_mesh = mesh vertices:vertarray faces:facearray 
		update new_mesh

		progressEnd()
	)
	
)
