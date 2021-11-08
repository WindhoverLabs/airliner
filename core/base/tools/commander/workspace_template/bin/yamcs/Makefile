.PHONE: deploy remove show-images

image:
	@if [ -z ${IMAGE_NAME}]; then echo "IMAGE_NAME is not set. Aborting."; exit 1; fi
	@echo "Creating image '${IMAGE_NAME}'"
	@docker build --force-rm -t "${IMAGE_NAME}" .

remove-image:
	@if [ -z ${IMAGE_NAME}]; then echo "IMAGE_NAME is not set. Aborting."; exit 1; fi
	@docker rmi ${IMAGE_NAME}
	@docker image prune -f

remove-container:
	@if [ -z ${CONTAINER_NAME}]; then echo "CONTAINER_NAME is not set. Aborting."; exit 1; fi
	@docker container rm ${CONTAINER_NAME}


show-images:
	@docker container ls -all

container:
	@if [-z ${IMAGE_NAME}]; then echo "IMAGE_NAME is not set. Aborting."; exit 1; fi
	@if [-z ${YAMCS_INSTALL_DIR}]; then echo "YAMCS_INSTALL_DIR is not set. Aborting."; exit 1; fi
	@if [-z ${CONTAINER_NAME}]; then echo "CONTAINER}_NAME is not set. Aborting."; exit 1; fi
	@if [-z ${YAMCS_WORKSPACE_DIR}]; then echo "YAMCS_WORKSPACE_DIR is not set. Aborting."; exit 1; fi
	@if [-z ${YAMCS_PORT}]; then echo "YAMCS_PORT is not set. Aborting."; exit 1; fi
	@if [-z ${TLM_PORT}]; then echo "TLM_PORT is not set. Aborting."; exit 1; fi
	
	@docker run \
		-d \
		--restart unless-stopped \
		-p ${YAMCS_PORT}:8090 \
		-p ${TLM_PORT}:1235/udp \
		--volume ${YAMCS_INSTALL_DIR}:/opt/yamcs \
		--volume ${YAMCS_WORKSPACE_DIR}:/opt/yamcs_workspace \
		--name ${CONTAINER_NAME} \
		-t -i \
		${IMAGE_NAME}

deploy-example:
	@make image IMAGE_NAME=yamcs-example:1.0.0
	@make container IMAGE_NAME=yamcs-example:1.0.0 CONTAINER_NAME=rig3 YAMCS_INSTALL_DIR=/opt/yamcs/5.4.3 YAMCS_WORKSPACE_DIR=/etc/yamcs/workspaces/rig3 YAMCS_PORT=8093 TLM_PORT=2233
	@make container IMAGE_NAME=yamcs-example:1.0.0 CONTAINER_NAME=rig4 YAMCS_INSTALL_DIR=/opt/yamcs/5.4.3 YAMCS_WORKSPACE_DIR=/etc/yamcs/workspaces/rig4 YAMCS_PORT=8094 TLM_PORT=2234
	@make container IMAGE_NAME=yamcs-example:1.0.0 CONTAINER_NAME=rig5 YAMCS_INSTALL_DIR=/opt/yamcs/5.4.3 YAMCS_WORKSPACE_DIR=/etc/yamcs/workspaces/rig5 YAMCS_PORT=8095 TLM_PORT=2235

deploy-airliner:
	@make image IMAGE_NAME=yamcs-airliner:5.5.4
	@make container YAMCS_PORT=8090 YAMCS_INSTALL_DIR=/opt/yamcs-develop/ YAMCS_WORKSPACE_DIR=/etc/yamcs/workspaces/develop TLM_PORT=5011 CONTAINER_NAME=airliner-develop IMAGE_NAME=yamcs-airliner:5.5.4

destroy:
	@docker container stop rig1 || true
	@docker container stop rig1 || true
	@docker container stop rig1 || true
	@docker container stop rig1 || true
	@docker container stop rig1 || true
	@make remove-container CONTAINER_NAME=rig1 || true
	@make remove-container CONTAINER_NAME=rig2 || true
	@make remove-container CONTAINER_NAME=rig3 || true
	@make remove-container CONTAINER_NAME=rig4 || true
	@make remove-container CONTAINER_NAME=rig5 || true
	@rm -Rf /opt/yamcs/worksapces/rig1/cache
	@rm -Rf /opt/yamcs/worksapces/rig2/cache
	@rm -Rf /opt/yamcs/worksapces/rig3/cache
	@rm -Rf /opt/yamcs/worksapces/rig4/cache
	@rm -Rf /opt/yamcs/worksapces/rig5/cache
	@rm -Rf /opt/yamcs/worksapces/rig1/yamcs-data
	@rm -Rf /opt/yamcs/worksapces/rig2/yamcs-data
	@rm -Rf /opt/yamcs/worksapces/rig3/yamcs-data
	@rm -Rf /opt/yamcs/worksapces/rig4/yamcs-data
	@rm -Rf /opt/yamcs/worksapces/rig5/yamcs-data
